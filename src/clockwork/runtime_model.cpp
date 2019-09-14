#include <dmlc/logging.h>
#include "clockwork/runtime_model.h"

using namespace clockwork;

RuntimeModel::RuntimeModel(PageCache* cache, model::ColdModel* cold) : cache(cache), cold(cold) {
	params_callback = new ParamsEvictionCallback(this);
	workspace_callback = new WorkspaceEvictionCallback(this);
}

RuntimeModel::State RuntimeModel::lock() {
	if (!cache->trylock(params_alloc)) return State::Warm;
	if (!cache->trylock(workspace_alloc)) return State::Hot;
	return State::Exec;
}

void RuntimeModel::unlock() {
	cache->unlock(workspace_alloc);
	cache->unlock(params_alloc);
}

int RuntimeModel::inputsize() {
	return warm->inputsize();
}

int RuntimeModel::outputsize() {
	return warm->outputsize();
}

void RuntimeModel::coldToCool() {
	CHECK(cold != nullptr) << "Cannot transition cold -> cool, cold == nullptr";
	CHECK(cool == nullptr) << "Cannot transition cold -> cool, cool already exists";
	cool = cold->load();
}

void RuntimeModel::coolToWarm() {
	CHECK(cool != nullptr) << "Cannot transition cool -> warm, cool == nullptr";
	CHECK(warm == nullptr) << "Cannot transition cool -> warm, cool already exists";
	warm = cool->load();
}

void RuntimeModel::warmToHot() {
	CHECK(warm != nullptr) << "Cannot transition warm -> hot, warm == nullptr";
	CHECK(hot == nullptr) << "Cannot transition warm -> hot, hot != nullptr";
	CHECK(params_alloc == nullptr) << "Cannot transition warm -> hot, params_alloc already allocated";
	params_alloc = cache->alloc(warm->num_params_pages(cache->page_size), params_callback);
	std::vector<char*> page_ptrs(params_alloc->pages.size());
	for (unsigned i = 0; i < params_alloc->pages.size(); i++) {
		page_ptrs[i] = params_alloc->pages[i]->ptr;
	}
	hot = warm->load(page_ptrs);
}

void RuntimeModel::hotToExec() {
	CHECK(hot != nullptr) << "Cannot transition hot -> exec, hot == nullptr";
	CHECK(exec == nullptr) << "Cannot transition hot -> exec, exec != nullptr";
	CHECK(workspace_alloc == nullptr) << "Cannot transition hot -> exec, workspace_alloc already allocated";
	workspace_alloc = cache->alloc(hot->num_workspace_pages(cache->page_size), workspace_callback);
	std::vector<char*> page_ptrs(workspace_alloc->pages.size());
	for (unsigned i = 0; i < workspace_alloc->pages.size(); i++) {
		page_ptrs[i] = workspace_alloc->pages[i]->ptr;
	}
	exec = hot->load(page_ptrs);
}

void RuntimeModel::setInput(void* input) {
	CHECK(exec != nullptr) << "Cannot set input on exec == nullptr";
	exec->setinput(input);
}

void RuntimeModel::call() {
	CHECK(exec != nullptr) << "Cannot call exec == nullptr";
	exec->call();
}

void RuntimeModel::getOutput(void* output) {
	CHECK(exec != nullptr) << "Cannot get output of exec == nullptr";
	exec->getoutput(output);
}

void RuntimeModel::execToHot() {
	CHECK(exec != nullptr) << "Cannot transition exec -> hot, exec == nullptr";
	CHECK(workspace_alloc != nullptr) << "Cannot transition exec -> hot, workspace_alloc == nullptr";
	workspace_alloc = nullptr;
	exec->unload();
	exec = nullptr;
}

void RuntimeModel::hotToWarm() {
	CHECK(hot != nullptr) << "Cannot transition hot -> warm, hot == nullptr";
	CHECK(params_alloc != nullptr) << "Cannot transition hot -> warm, params_alloc == nullptr";
	params_alloc = nullptr;
	exec->unload();
	exec = nullptr;
}

void RuntimeModel::warmToCool() {
	CHECK(warm != nullptr) << "Cannot transition warm -> cool, warm == nullptr";
	warm->unload();
	warm = nullptr;
}

void RuntimeModel::coolToCold() {
	CHECK(cool != nullptr) << "Cannot transition cool -> cold, cool == nullptr";
	cool->unload();
	cool = nullptr;
}