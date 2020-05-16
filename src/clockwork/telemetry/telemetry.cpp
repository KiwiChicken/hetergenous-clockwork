#include "clockwork/telemetry/controller_request_logger.h"
#include "clockwork/telemetry/controller_action_logger.h"

namespace clockwork {

RequestTelemetryFileLogger::RequestTelemetryFileLogger(std::string filename) : f(filename) {
	write_headers();
}

void RequestTelemetryFileLogger::write_headers() {
	f << "t" << "\t";
	f << "request_id" << "\t";
	f << "result" << "\t";
	f << "user_id" << "\t";
	f << "model_id" << "\t";
	f << "slo_factor" << "\t";
	f << "latency" << "\t";
	f << "deadline" << "\t";
	f << "deadline_met" << "\n";
}

void RequestTelemetryFileLogger::log(ControllerRequestTelemetry &t) {
	f << t.departure << "\t";
	f << t.request_id << "\t";
	f << t.result << "\t";
	f << t.user_id << "\t";
	f << t.model_id << "\t";
	f << t.slo_factor << "\t";
	f << (t.departure - t.arrival) << "\t";

	int64_t deadline;
	bool deadline_met;
	if (t.deadline == 0) {
		deadline = -1;
		deadline_met = t.result == clockworkSuccess;
	} else if (t.deadline < t.arrival) {
		deadline = 0;
		deadline_met = false;
	} else {
		deadline = t.deadline - t.arrival; // Print the deadline relative to arrival time
		deadline_met = t.result == clockworkSuccess && t.departure <= t.deadline;
	}
	f << deadline << "\t";
	f << deadline_met << "\n";
}

void RequestTelemetryFileLogger::shutdown(bool awaitCompletion) {
	f.close();
}

AsyncRequestTelemetryLogger::AsyncRequestTelemetryLogger() {}

void AsyncRequestTelemetryLogger::addLogger(RequestTelemetryLogger* logger) {
	loggers.push_back(logger);
}

void AsyncRequestTelemetryLogger::start() {
	thread = std::thread(&AsyncRequestTelemetryLogger::run, this);
	threading::initLoggerThread(thread);
}

void AsyncRequestTelemetryLogger::run() {
	while (alive) {
		ControllerRequestTelemetry next;
		while (queue.try_pop(next)) {
			for (auto &logger : loggers) {
				logger->log(next);
			}
		}

		usleep(1000);
	}
}

void AsyncRequestTelemetryLogger::log(ControllerRequestTelemetry &telemetry) {
	queue.push(telemetry);
}

void AsyncRequestTelemetryLogger::shutdown(bool awaitCompletion) {
	alive = false;
	for (auto & logger : loggers) {
		logger->shutdown(true);
	}
}

RequestTelemetryPrinter::RequestTelemetryPrinter(uint64_t print_interval) :
	print_interval(print_interval) {}

void RequestTelemetryPrinter::print(uint64_t interval) {
	if (buffered.size() == 0) {
		std::stringstream ss;
		ss << "Client throughput=0" << std::endl;
		std::cout << ss.str();
		return;
	}

	uint64_t duration_sum = 0;
	unsigned count = 0;
	unsigned violations = 0;
	uint64_t min_latency = UINT64_MAX;
	uint64_t max_latency = 0;
	while (buffered.size() > 0) {
		ControllerRequestTelemetry &next = buffered.front();

		if (next.result == clockworkSuccess) {
			uint64_t latency = (next.departure - next.arrival);
			duration_sum += latency;
			count++;
			min_latency = std::min(min_latency, latency);
			max_latency = std::max(max_latency, latency);
		} else {
			violations++;
		}

		buffered.pop();
	}

	double throughput = (1000000000.0 * count) / ((double) interval);
	double success_rate = 100;
	if (count > 0 || violations > 0) {
		success_rate = count / ((double) (count + violations));
	}

	std::stringstream ss;
	ss << std::fixed;
	if (count == 0) {
		ss << "Client throughput=0 success=0% (" << violations << "/" << violations << " violations)";
	} else {
		ss << "Client throughput=" << std::setprecision(1) << throughput;
		ss << " success=" << std::setprecision(2) << (100*success_rate) << "%";
		if (violations > 0) {
			ss << " (" << violations << "/" << (count+violations) << " violations)";
		}
		ss << " min=" << std::setprecision(1) << (min_latency / 1000000.0);
		ss << " max=" << std::setprecision(1) << (max_latency / 1000000.0);
		ss << " mean=" << std::setprecision(1) << ((duration_sum/count) / 1000000.0);
	}
	ss << std::endl;
	std::cout << ss.str();
}

void RequestTelemetryPrinter::log(ControllerRequestTelemetry &telemetry) {
	buffered.push(telemetry);

	uint64_t now = util::now();
	if (last_print + print_interval <= now) {
		print(now - last_print);
		last_print = now;
	}
}

void RequestTelemetryPrinter::shutdown(bool awaitCompletion) {
	std::cout << "RequestTelemetryPrinter shutting down" << std::endl;
	std::cout << std::flush;
}

RequestTelemetryLogger* ControllerRequestTelemetry::summarize(uint64_t print_interval) {
	auto result = new AsyncRequestTelemetryLogger();
	result->addLogger(new RequestTelemetryPrinter(print_interval));
	result->start();
	return result;
}

RequestTelemetryLogger* ControllerRequestTelemetry::log_and_summarize(std::string filename, uint64_t print_interval) {
	auto result = new AsyncRequestTelemetryLogger();
	result->addLogger(new RequestTelemetryFileLogger(filename));
	result->addLogger(new RequestTelemetryPrinter(print_interval));
	result->start();
	return result;
}

void ControllerRequestTelemetry::set(clientapi::InferenceRequest &request) {
	if (request.arrival == 0) {
		request.arrival = util::now();
	}
	arrival = request.arrival;
	request_id = request.header.user_request_id;
	user_id = request.header.user_id;
	model_id = request.model_id;
	slo_factor = request.slo_factor;
}

void ControllerRequestTelemetry::set(clientapi::InferenceResponse &response) {
	if (response.departure == 0) {
		response.departure = util::now();
	}
	departure = response.departure;
	result = response.header.status;
	deadline = response.deadline;
}

void ControllerActionTelemetry::set(std::shared_ptr<workerapi::Infer> &infer) {
	action_id = infer->id;
	gpu_id = infer->gpu_id;
	action_type = workerapi::inferAction;
	batch_size = infer->batch_size;
	model_id = infer->model_id;
	earliest = infer->earliest;
	latest = infer->latest;
	expected_duration = infer->expected_duration;
	expected_gpu_clock = infer->expected_gpu_clock;
	action_sent = util::now();
}

void ControllerActionTelemetry::set(std::shared_ptr<workerapi::LoadWeights> &load) {
	action_id = load->id;
	gpu_id = load->gpu_id;
	action_type = workerapi::loadWeightsAction;
	batch_size = 0;
	model_id = load->model_id;
	earliest = load->earliest;
	latest = load->latest;
	expected_duration = load->expected_duration;
	action_sent = util::now();
}

void ControllerActionTelemetry::set(std::shared_ptr<workerapi::EvictWeights> &evict) {
	action_id = evict->id;
	gpu_id = evict->gpu_id;
	action_type = workerapi::evictWeightsAction;
	batch_size = 0;
	model_id = evict->model_id;
	earliest = evict->earliest;
	latest = evict->latest;
	expected_duration = 0;
	action_sent = util::now();
}

void ControllerActionTelemetry::set(std::shared_ptr<workerapi::ErrorResult> &result) {
	result_received = util::now();
	status = result->status;
	worker_duration = 0;
	worker_exec_complete = 0;
}

void ControllerActionTelemetry::set(std::shared_ptr<workerapi::InferResult> &result) {
	result_received = util::now();
	status = result->status;
	worker_duration = result->exec.duration;
	worker_exec_complete = result->exec.end;
	gpu_clock = result->gpu_clock;
}

void ControllerActionTelemetry::set(std::shared_ptr<workerapi::LoadWeightsResult> &result) {
	result_received = util::now();
	status = result->status;
	worker_duration = result->duration;
}

void ControllerActionTelemetry::set(std::shared_ptr<workerapi::EvictWeightsResult> &result) {
	result_received = util::now();
	status = result->status;
	worker_duration = result->duration;
}

AsyncControllerActionTelemetryLogger* ControllerActionTelemetry::summarize(uint64_t print_interval) {
	auto result = new AsyncControllerActionTelemetryLogger();
	result->addLogger(new SimpleActionPrinter(print_interval));
	result->start();
	return result;
}

AsyncControllerActionTelemetryLogger* ControllerActionTelemetry::log_and_summarize(std::string filename, uint64_t print_interval) {
	auto result = new AsyncControllerActionTelemetryLogger();
	result->addLogger(new SimpleActionPrinter(print_interval));
	result->addLogger(new ControllerActionTelemetryFileLogger(filename));
	result->start();
	return result;
}

ControllerActionTelemetryFileLogger::ControllerActionTelemetryFileLogger(std::string filename) : f(filename) {
	write_headers();
}

void ControllerActionTelemetryFileLogger::write_headers() {
	f << "t" << "\t";
	f << "action_id" << "\t";
	f << "action_type" << "\t";
	f << "status" << "\t";
	f << "worker_id" << "\t";
	f << "gpu_id" << "\t";
	f << "model_id" << "\t";
	f << "batch_size" << "\t";
	f << "controller_action_duration" << "\t";
	f << "expected_exec_duration" << "\t";
	f << "worker_exec_duration" << "\t";
	f << "expected_exec_complete" << "\t";
	f << "worker_exec_complete" << "\t";
	f << "expected_gpu_clock" << "\t";
	f << "worker_gpu_clock" << "\t";
	f << "goodput" << "\n";
}

void ControllerActionTelemetryFileLogger::log(ControllerActionTelemetry &t) {
	f << t.result_received << "\t";
	f << t.action_id << "\t";
	f << t.action_type << "\t";
	f << t.status << "\t";
	f << t.worker_id << "\t";
	f << t.gpu_id << "\t";
	f << t.model_id << "\t";
	f << t.batch_size << "\t";
	f << (t.result_received - t.action_sent) << "\t";
	f << t.expected_duration << "\t";
	f << t.worker_duration << "\t";
	if (t.expected_exec_complete == 0) {
		f << "0\t";
	} else {
		f << (t.expected_exec_complete - t.action_sent) << "\t";
	}
	if (t.worker_exec_complete == 0) {
		f << "0\t";
	} else {
		f << (t.worker_exec_complete - t.action_sent) << "\t";
	}
	f << t.expected_gpu_clock << "\t";
	f << t.gpu_clock << "\t";
	f << static_cast<uint64_t>(t.worker_duration * t.goodput) << "\n";
}

void ControllerActionTelemetryFileLogger::shutdown(bool awaitCompletion) {
	f.close();
}

AsyncControllerActionTelemetryLogger::AsyncControllerActionTelemetryLogger() {}


void AsyncControllerActionTelemetryLogger::addLogger(ControllerActionTelemetryLogger* logger) {
	loggers.push_back(logger);
}

void AsyncControllerActionTelemetryLogger::start() {
	this->thread = std::thread(&AsyncControllerActionTelemetryLogger::run, this);
	threading::initLoggerThread(thread);
}

void AsyncControllerActionTelemetryLogger::run() {
	while (alive) {
		ControllerActionTelemetry next;
		while (queue.try_pop(next)) {
			for (auto &logger : loggers) {
				logger->log(next);
			}
		}

		usleep(1000);
	}
}

void AsyncControllerActionTelemetryLogger::log(ControllerActionTelemetry &telemetry) {
	queue.push(telemetry);
}

void AsyncControllerActionTelemetryLogger::shutdown(bool awaitCompletion) {
	alive = false;
	for (auto & logger : loggers) {
		logger->shutdown(true);
	}
}

ActionPrinter::ActionPrinter(uint64_t print_interval) : print_interval(print_interval) {}

void ActionPrinter::log(ControllerActionTelemetry &telemetry) {
	buffered.push(telemetry);

	uint64_t now = util::now();
	if (last_print + print_interval <= now) {
		print(now - last_print, buffered);
		last_print = now;
	}
}

void ActionPrinter::shutdown(bool awaitCompletion) {}


SimpleActionPrinter::SimpleActionPrinter(uint64_t print_interval) : ActionPrinter(print_interval) {}

std::map<SimpleActionPrinter::Group, std::queue<ControllerActionTelemetry>> make_groups(
		std::queue<ControllerActionTelemetry> &buffered) {
	std::map<SimpleActionPrinter::Group, std::queue<ControllerActionTelemetry>> result;
	while (!buffered.empty()) {
		auto &t = buffered.front();
		if ((t.action_type == workerapi::loadWeightsAction || 
			t.action_type == workerapi::inferAction) &&
			t.status == clockworkSuccess) {
			SimpleActionPrinter::Group key = std::make_tuple(t.worker_id, t.gpu_id, t.action_type);
			result[key].push(t);
		}
		buffered.pop();
	}
	return result;
}

void SimpleActionPrinter::print(uint64_t interval, std::queue<ControllerActionTelemetry> &buffered) {
	auto groups = make_groups(buffered);

	for (auto &p : groups) {
		print(interval, p.first, p.second);
	}
}

struct Stat {
	std::vector<uint64_t> v;

	unsigned size() { return v.size(); }
	uint64_t min() { return *std::min_element(v.begin(), v.end()); }
	uint64_t max() { return *std::max_element(v.begin(), v.end()); }
	uint64_t mean() { return std::accumulate(v.begin(), v.end(), 0.0) / v.size(); }
	double throughput(uint64_t interval) {
		return (size() * 1000000000.0) / static_cast<double>(interval);
	}
	double utilization(uint64_t interval) { 
		return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(interval);
	}
};

void SimpleActionPrinter::print(uint64_t interval, const Group &group, std::queue<ControllerActionTelemetry> &buffered) {
	if (buffered.empty()) return;

	int worker_id = std::get<0>(group);
	int gpu_id = std::get<1>(group);
	int action_type = std::get<2>(group);

	Stat e2e;
	Stat w;
	Stat w_norm;
	Stat gpu_clock;

	if (buffered.empty()) {
		std::stringstream s;
		s << std::fixed << std::setprecision(2);
		s << "W" << worker_id
		  << "-GPU" << gpu_id
		  << " throughput=0" << std::endl;
		std::cout << s.str();
		return;
	}

	while (!buffered.empty()) {
		auto &t = buffered.front();
		e2e.v.push_back(t.result_received - t.action_sent);
		w.v.push_back(t.worker_duration);
		w_norm.v.push_back(t.worker_duration * t.gpu_clock / 1380);
		gpu_clock.v.push_back(t.gpu_clock);
		buffered.pop();
	}

	std::stringstream s;
	s << std::fixed << std::setprecision(2);
	s << "W" << worker_id
	  << "-GPU" << gpu_id;

	switch(action_type) {
		case workerapi::loadWeightsAction: s << " LoadW"; break;
		case workerapi::inferAction: s << " Infer"; break;
		default: return;
	}

	s << " min=" << (w.min() / 1000000.0)
	  << " max=" << (w.max() / 1000000.0)
	  << " mean=" << (w.mean() / 1000000.0) 
	  << " e2emean=" << (e2e.mean() / 1000000.0)
	  << " e2emax=" << (e2e.max() / 1000000.0)
	  << std::setprecision(1)
	  << " throughput=" << w.throughput(interval) 
	  << std::setprecision(2)
	  << " utilization=" << w.utilization(interval)
	  << " clock=[" << gpu_clock.min() << "-" << gpu_clock.max() << "]"
	  << " norm_max=" << (w_norm.max() / 1000000.0)
	  << " norm_mean=" << (w_norm.mean() / 1000000.0)
	  << std::endl;
	std::cout << s.str();
}


}