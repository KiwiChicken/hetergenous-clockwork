#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "clockwork/telemetry.h"
#include "clockwork/common.h"
#include <fstream>
#include <sstream>
#include <pods/pods.h>
#include <pods/binary.h>
#include <pods/buffers.h>
#include <pods/streams.h>

/** Inflates output clockwork telemetry to TSV file */
void inflate(std::string input_filename, std::string output_filename) {
	std::ifstream infile;
	infile.open(input_filename);

    pods::InputStream in(infile);
    pods::BinaryDeserializer<decltype(in)> deserializer(in);

    uint64_t start_time = 0;

    clockwork::SerializedRequestTelemetry t;
    int count = 0;

    std::vector<std::string> headers = {{
    	"request_id",
    	"model_id",
    	"arrived", 
    	"submitted",
    	"complete",
    	"execution_latency",
    	"total_latency",
    }};

    for (unsigned i = 0; i < clockwork::TaskTypes.size(); i++) {
		std::string task_type = clockwork::TaskTypeName(clockwork::TaskTypes[i]);
		headers.push_back(task_type);
		headers.push_back(task_type + "_queue");
		headers.push_back(task_type + "_sync");
		headers.push_back(task_type + "_async");
    }

    std::vector<std::unordered_map<std::string, std::string>> rows;
    while (deserializer.load(t) == pods::Error::NoError) {	
		std::unordered_map<std::string, std::string> row;

		row["request_id"] = std::to_string(t.request_id);
		row["model_id"] = std::to_string(t.model_id);
		row["arrived"] = std::to_string(t.arrived);
		row["submitted"] = std::to_string(t.submitted);
		row["complete"] = std::to_string(t.complete);
		row["execution_latency"] = std::to_string(t.complete - t.submitted);
		row["total_latency"] = std::to_string(t.complete - t.arrived);

		for (unsigned i = 0; i < t.tasks.size(); i++) {
			clockwork::SerializedTaskTelemetry task = t.tasks[i];
			std::string task_type = clockwork::TaskTypeName(clockwork::TaskTypes[task.task_type]);

			if (i < t.tasks.size() - 1) {
				row[task_type] = std::to_string(t.tasks[i+1].enqueued - task.dequeued);
			} else {
				row[task_type] = std::to_string(t.complete - task.dequeued);
			}
			row[task_type + "_queue"] = std::to_string(task.dequeued - task.eligible_for_dequeue);
			row[task_type + "_sync"] = std::to_string(task.exec_complete - task.dequeued);
			row[task_type + "_async"] = std::to_string(task.async_duration);
		}

		rows.push_back(row);
    }
    std::cout << "Processed " << rows.size() << " records" << std::endl;

    std::ofstream outfile;
    outfile.open(output_filename);

    int i = 0;
	for (auto header : headers) {
		if (i++ > 0) {
			outfile << "\t";
		}
		outfile << header;
	}
	outfile << "\n";

    for (auto row : rows) {
    	i = 0;
    	for (auto header : headers) {
    		if (i++ > 0) outfile << "\t";
    		if (row.find(header) != row.end()) outfile << row[header];
    	}
    	outfile << "\n";
    }

    outfile.close();
}

void show_usage() {
	std::cout << "Inflates a binary format telemetry file into a TSV" << std::endl;
	std::cout << "Specify input filename and output filename" << std::endl;
}

int main(int argc, char *argv[]) {
	std::vector<std::string> non_argument_strings;

	for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {
		    show_usage();
		    return 0;
		} else {
		  non_argument_strings.push_back(arg);
		}
	}

	if (non_argument_strings.size() < 1) {
		std::cerr << "Expected input telemetry filename, none given." << std::endl 
		          << "Execute with --help for usage information." << std::endl;
		return 1;
	}

	std::string input_filename = non_argument_strings[0];
	std::string output_filename = input_filename + ".tsv";
	if (non_argument_strings.size() >= 2) {
		output_filename = non_argument_strings[1];
	}

	std::cout << "Inflating " << input_filename << std::endl
	          << "       to " << output_filename << std::endl;

	inflate(input_filename, output_filename);

	return 0;
}
