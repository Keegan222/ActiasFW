/*
File:		Logging.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.05@19:07
Purpose:	Contains the console and file logging utility for ActiasFW
			applications
*/

#ifndef ACTIASFW_LOGGING_H
#define ACTIASFW_LOGGING_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

namespace ActiasFW {
#define ACTIASFW_LOG(SRC, LVL, ...) Application::Log, \
	Application::Log.getTimestamp(), ": [", #SRC, "] [", #LVL, "] ", \
	__VA_ARGS__, "\n"

	// The logging system for ActiasFW applications
	class LogManager {
	public:
		/*
		Initialize this log manager's memory
		Parameter: bool enabled - Whether this log manager will outputs logs
		Parameter: const std::vector<std::string>& outputFileNames - The set of
		files to write logs to
		Parameter: const std::string& timestampFormat - The format to use when
		writing the date and time on logs
		Returns: bool - Whether this log manager was initialized successfully
		*/
		bool initialize(bool, const std::vector<std::string>&,
			const std::string&);
		/*
		Write an object as a string to this log manager's log
		Parameter: const T& data - An object of any type to write
		*/
		template <typename T>
		void write(const T& data) {
			if (m_enabled) {
				std::cout << data;
				for (std::map<std::string, std::ofstream>::iterator it
					= m_outputFiles.begin(); it != m_outputFiles.end(); it++) {
					if (it->second.good()) {
						it->second << data;
					}
				}
			}
		}
		/*
		Get the current date and time as a string in the timestamp format
		Returns: const std::string - The timestamp
		*/
		const std::string getTimestamp() const;
		/*
		Close this log manager's output files and free its memory
		Returns: bool - Whether this log manager was successfully destroyed
		*/
		bool destroy();
		/*
		Test whether this log manager is enabled
		Returns: bool - Whether this log manager is enabled
		*/
		bool isEnabled() const;
		/*
		Set whether this log manager is enabled
		Parameter: bool enabled - Whether to enable this log manager
		*/
		void setEnabled(bool);
		/*
		Get the set of names of the output files of this log manager
		Returns: std::vector<std::string> - The set of output file names
		*/
		std::vector<std::string> getOutputFileNames() const;
		/*
		Add an output file to this log manager by its file name
		Parameter: const std::string& outputFileName - The file name to add
		Returns: bool - Whether the file name was not in memory already and was
		available for writing
		*/
		bool addOutputFileName(const std::string&);
		/*
		Remove an output file from this log manager by its file name
		Parameter: const std::string& outputFileName - The file name to remove
		Returns: bool - Whether the file name was in memory and was closed and
		removed successfully
		*/
		bool removeOutputFileName(const std::string&);
		/*
		Get the current format for producing timestamps in logs
		Returns: const std::string& - The timestamp format
		*/
		const std::string& getTimestampFormat() const;
		/*
		Set the format for producing timestamps in logs
		Parameter: const std::string& timestampFormat - The new timestamp format
		Returns: bool - Whether the new timestamp format was valid
		*/
		bool setTimestampFormat(const std::string&);

	private:
		// Whether this log manager has been initialized
		bool m_initialized = false;
		// Whether this log manager is currently enabled
		bool m_enabled = false;
		// The set of output files for this log manager associated with their
		// file names
		std::map<std::string, std::ofstream> m_outputFiles;
		// The format to write timestamps with in logs
		std::string m_timestampFormat = "";
	};

	/*
	Overload for the , operator on LogManagers, used to write any object to a
	log manager
	Parameter: LogManager& log - The log manager to write an object to
	Parameter: const T& data - The object to write
	Returns: LogManager& - The log manager written to, used to chain multiple
	objects together as a log
	*/
	template <typename T>
	LogManager& operator , (LogManager& log, const T& data) {
		log.write(data);
		return log;
	}
}

#endif