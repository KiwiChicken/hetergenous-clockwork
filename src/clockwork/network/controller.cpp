#include "clockwork/network/controller.h"

namespace clockwork {
namespace network {
namespace controller {

WorkerConnection::WorkerConnection(asio::io_service &io_service, workerapi::Controller* controller) :
		message_connection(io_service, *this),
		msg_tx_(this, *this),
		controller(controller),
		connected(false) {
}

message_rx* WorkerConnection::new_rx_message(message_connection *tcp_conn, uint64_t header_len,
		uint64_t body_len, uint64_t msg_type, uint64_t msg_id) {
	using namespace clockwork::workerapi;

	if (msg_type == RES_ERROR) {
		auto msg = new error_result_rx();
		msg->set_msg_id(msg_id);
		return msg;

	} else if (msg_type == RES_LOAD_MODEL_FROM_DISK) {
		auto msg = new load_model_from_disk_result_rx();
		msg->set_msg_id(msg_id);
		return msg;

	} else if (msg_type == RES_LOAD_WEIGHTS) {
		auto msg = new load_weights_result_rx();
		msg->set_msg_id(msg_id);
		return msg;

	} else if (msg_type == RES_INFER) {
		auto msg = new infer_result_rx();
		msg->set_body_len(body_len);
		msg->set_msg_id(msg_id);
		return msg;

	} else if (msg_type == RES_EVICT_WEIGHTS) {
		auto msg = new evict_weights_result_rx();
		msg->set_msg_id(msg_id);
		return msg;

	}
	
	CHECK(false) << "Unsupported msg_type " << msg_type;
	return nullptr;
}

void WorkerConnection::ready() {
	connected.store(true);
}

void WorkerConnection::closed() {
}

void WorkerConnection::aborted_receive(message_connection *tcp_conn, message_rx *req) {
	delete req;
}

void WorkerConnection::completed_receive(message_connection *tcp_conn, message_rx *req) {
	if (auto error = dynamic_cast<error_result_rx*>(req)) {
		auto result = std::make_shared<workerapi::ErrorResult>();
		error->get(*result);
		controller->sendResult(result);

	} else if (auto load_model = dynamic_cast<load_model_from_disk_result_rx*>(req)) {
		auto result = std::make_shared<workerapi::LoadModelFromDiskResult>();
		load_model->get(*result);
		controller->sendResult(result);

	} else if (auto load_weights = dynamic_cast<load_weights_result_rx*>(req)) {
		auto result = std::make_shared<workerapi::LoadWeightsResult>();
		load_weights->get(*result);
		controller->sendResult(result);
		
	} else if (auto infer = dynamic_cast<infer_result_rx*>(req)) {
		auto result = std::make_shared<workerapi::InferResult>();
		infer->get(*result);
		controller->sendResult(result);
		
	} else if (auto evict = dynamic_cast<evict_weights_result_rx*>(req)) {
		auto result = std::make_shared<workerapi::EvictWeightsResult>();
		evict->get(*result);
		controller->sendResult(result);
		
	} else {
		CHECK(false) << "Received an unsupported message_rx type";
	}

	delete req;
}

void WorkerConnection::completed_transmit(message_connection *tcp_conn, message_tx *req) {
	delete req;
}

void WorkerConnection::aborted_transmit(message_connection *tcp_conn, message_tx *req) {
	delete req;
}

void WorkerConnection::sendActions(std::vector<std::shared_ptr<workerapi::Action>> &actions) {
	for (auto &action : actions) {
		sendAction(action);
	}
}

void WorkerConnection::sendAction(std::shared_ptr<workerapi::Action> &action) {
	if (auto load_model = std::dynamic_pointer_cast<workerapi::LoadModelFromDisk>(action)) {
		auto tx = new load_model_from_disk_action_tx();
		tx->set(*load_model);
		msg_tx_.send_message(*tx);

	} else if (auto load_weights = std::dynamic_pointer_cast<workerapi::LoadWeights>(action)) {
		auto tx = new load_weights_action_tx();
		tx->set(*load_weights);
		msg_tx_.send_message(*tx);

	} else if (auto infer = std::dynamic_pointer_cast<workerapi::Infer>(action)) {
		auto tx = new infer_action_tx();
		tx->set(*infer);
		msg_tx_.send_message(*tx);

	} else if (auto evict_weights = std::dynamic_pointer_cast<workerapi::EvictWeights>(action)) {
		auto tx = new evict_weights_action_tx();
		tx->set(*evict_weights);
		msg_tx_.send_message(*tx);

	} else {
		CHECK(false) << "Sending an unsupported action type";
	}
}

WorkerManager::WorkerManager() : alive(true), network_thread(&WorkerManager::run, this) {}

void WorkerManager::run() {
	while (alive) {
		try {
			asio::io_service::work work(io_service);
			io_service.run();
		} catch (std::exception& e) {
			alive.store(false);
			CHECK(false) << "Exception in network thread: " << e.what();
		} catch (const char* m) {
			alive.store(false);
			CHECK(false) << "Exception in network thread: " << m;
		}
	}
}

void WorkerManager::shutdown(bool awaitCompletion) {
	alive.store(false);
	io_service.stop();
	if (awaitCompletion) {
		join();
	}
}

void WorkerManager::join() {
	network_thread.join();
}

WorkerConnection* WorkerManager::connect(std::string host, std::string port, workerapi::Controller* controller) {
	try {
		WorkerConnection* c = new WorkerConnection(io_service, controller);
		c->connect(host, port);
		std::cout << "Connecting to worker " << host << ":" << port << std::endl;
		while (alive.load() && !c->connected.load()); // If connection fails, alive sets to false
		std::cout << "Connection established" << std::endl;
		return c;
	} catch (std::exception& e) {
		alive.store(false);
		io_service.stop();
		CHECK(false) << "Exception in network thread: " << e.what();
	} catch (const char* m) {
		alive.store(false);
		io_service.stop();
		CHECK(false) << "Exception in network thread: " << m;
	}
}

ClientConnection::ClientConnection(asio::io_service &io_service, clientapi::ClientAPI* api) :
		message_connection(io_service, *this),
		msg_tx_(this, *this),
		api(api),
		connected(false) {
}

message_rx* ClientConnection::new_rx_message(message_connection *tcp_conn, uint64_t header_len,
		uint64_t body_len, uint64_t msg_type, uint64_t msg_id) {
	using namespace clockwork::clientapi;

	if (msg_type == REQ_INFERENCE) {
		auto msg = new msg_inference_req_rx();
		msg->set_body_len(body_len);
		msg->set_msg_id(msg_id);
		return msg;

	} else if (msg_type == REQ_EVICT) {
		auto msg = new msg_evict_req_rx();
		msg->set_msg_id(msg_id);
		return msg;

	} else if (msg_type == REQ_LOAD_REMOTE_MODEL) {
		auto msg = new msg_load_remote_model_req_rx();
		msg->set_msg_id(msg_id);
		return msg;

	} else if (msg_type == REQ_UPLOAD_MODEL) {
		auto msg = new msg_upload_model_req_rx();
		msg->set_body_len(body_len);
		msg->set_msg_id(msg_id);
		return msg;

	}
	
	CHECK(false) << "Unsupported msg_type " << msg_type;
	return nullptr;
}

void ClientConnection::ready() {
	connected.store(true);
}

void ClientConnection::closed() {
}

void ClientConnection::aborted_receive(message_connection *tcp_conn, message_rx *req) {
	delete req;
}

void ClientConnection::completed_receive(message_connection *tcp_conn, message_rx *req) {
	int request_id = req->get_msg_id();

	if (auto infer = dynamic_cast<msg_inference_req_rx*>(req)) {
		auto request = new clientapi::InferenceRequest();
		infer->get(*request);
		api->infer(*request, [this, request, request_id] (clientapi::InferenceResponse &response) {
			delete request;
			auto rsp = new msg_inference_rsp_tx();
			rsp->set(response);
			rsp->set_msg_id(request_id);
			msg_tx_.send_message(*rsp);
		});

	} else if (auto evict = dynamic_cast<msg_evict_req_rx*>(req)) {
		auto request = new clientapi::EvictRequest();
		evict->get(*request);
		api->evict(*request, [this, request, request_id] (clientapi::EvictResponse &response) {
			delete request;
			auto rsp = new msg_evict_rsp_tx();
			rsp->set(response);
			rsp->set_msg_id(request_id);
			msg_tx_.send_message(*rsp);
		});

	} else if (auto load_model = dynamic_cast<msg_load_remote_model_req_rx*>(req)) {
		auto request = new clientapi::LoadModelFromRemoteDiskRequest();
		load_model->get(*request);
		api->loadRemoteModel(*request, [this, request, request_id] (clientapi::LoadModelFromRemoteDiskResponse &response) {
			delete request;
			auto rsp = new msg_load_remote_model_rsp_tx();
			rsp->set(response);
			rsp->set_msg_id(request_id);
			msg_tx_.send_message(*rsp);
		});
		
	} else if (auto upload_model = dynamic_cast<msg_upload_model_req_rx*>(req)) {
		auto request = new clientapi::UploadModelRequest();
		upload_model->get(*request);
		api->uploadModel(*request, [this, request, request_id] (clientapi::UploadModelResponse &response) {
			delete request;
			auto rsp = new msg_upload_model_rsp_tx();
			rsp->set(response);
			rsp->set_msg_id(request_id);
			msg_tx_.send_message(*rsp);
		});
		
	} else {
		CHECK(false) << "Received an unsupported RPC request";
	}

	delete req;
}

void ClientConnection::completed_transmit(message_connection *tcp_conn, message_tx *req) {
	delete req;
}

void ClientConnection::aborted_transmit(message_connection *tcp_conn, message_tx *req) {
	delete req;
}

Server::Server(clientapi::ClientAPI* api, int port) :
		api(api),
		network_thread(&Server::run, this, port),
		alive(true) {
}

void Server::shutdown(bool awaitShutdown) {
	io_service.stop();
	if (awaitShutdown) {
		join();
	}
}

void Server::join() {
	while (alive.load());
}

void Server::run(int port) {
	try {
		tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), port));
		start_accept(&acceptor);
		std::cout << "Running io service thread" << std::endl;
		io_service.run();
	} catch (std::exception& e) {
		CHECK(false) << "Exception in network thread: " << e.what();
	} catch (const char* m) {
		CHECK(false) << "Exception in network thread: " << m;
	}
	std::cout << "Server exiting" << std::endl;
	alive.store(false);
}

void Server::start_accept(tcp::acceptor* acceptor) {
	auto connection = new ClientConnection(acceptor->get_io_service(), api);

	acceptor->async_accept(connection->get_socket(),
		boost::bind(&Server::handle_accept, this, connection, acceptor,
			asio::placeholders::error));
}

void Server::handle_accept(ClientConnection* connection, tcp::acceptor* acceptor, const asio::error_code& error) {
	if (error) {
		throw std::runtime_error(error.message());
	}

	connection->established();
	start_accept(acceptor);
}



}
}
}