/*
File:		Logging.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.05@19:11
Purpose:	Defines variables and implements functions found in Logging.h
*/

#include "ActiasFW.h"

namespace ActiasFW {
	// Implement LogManager class functions

	bool LogManager::initialize(bool enabled,
		const std::vector<std::string>& outputFileNames,
		const std::string& timestampFormat) {
		if (m_initialized) {
			return false;
		}
		setEnabled(enabled);
		for (const std::string& outputFileName : outputFileNames) {
			if (!addOutputFileName(outputFileName)) {
				return false;
			}
		}
		if (!setTimestampFormat(timestampFormat)) {
			return false;
		}
		m_initialized = true;
		return true;
	}

	const std::string LogManager::getTimestamp() const {
		struct tm t;
		__time64_t lt;
		char buffer[64];
		errno_t err;
		_time64(&lt);
		err = _localtime64_s(&t, &lt);
		if (err) {
			return "";
		}
		strftime(buffer, sizeof(buffer), m_timestampFormat.c_str(), &t);
		return std::string(buffer);
	}

	bool LogManager::destroy() {
		if (!m_initialized) {
			return false;
		}
		m_initialized = false;
		m_enabled = false;
		for (std::map<std::string, std::ofstream>::iterator it
			= m_outputFiles.begin(); it != m_outputFiles.end(); it++) {
			it->second.close();
		}
		m_outputFiles.clear();
		m_timestampFormat = "";
		return true;
	}

	bool LogManager::isEnabled() const {
		return m_enabled;
	}

	void LogManager::setEnabled(bool enabled) {
		m_enabled = enabled;
	}

	std::vector<std::string> LogManager::getOutputFileNames() const {
		std::vector<std::string> outputFileNames;
		for (const std::pair<const std::string, std::ofstream>& pair
			: m_outputFiles) {
			outputFileNames.push_back(pair.first);
		}
		return outputFileNames;
	}

	bool LogManager::addOutputFileName(const std::string& outputFileName) {
		std::map<std::string, std::ofstream>::iterator it
			= m_outputFiles.find(outputFileName);
		if (it != m_outputFiles.end()) {
			return false;
		}
		std::ofstream outputFile(outputFileName);
		if (!outputFile.good()) {
			return false;
		}
		m_outputFiles[outputFileName] = std::ofstream(outputFileName);
		return true;
	}

	bool LogManager::removeOutputFileName(const std::string& outputFileName) {
		std::map<std::string, std::ofstream>::iterator it
			= m_outputFiles.find(outputFileName);
		if (it == m_outputFiles.end()) {
			return false;
		}
		it->second.close();
		m_outputFiles.erase(it);
		return true;
	}

	const std::string& LogManager::getTimestampFormat() const {
		return m_timestampFormat;
	}

	bool LogManager::setTimestampFormat(const std::string& timestampFormat) {
		std::string oldTimestampFormat = m_timestampFormat;
		m_timestampFormat = timestampFormat;
		if (!getTimestamp().empty()) {
			return true;
		}
		else {
			m_timestampFormat = oldTimestampFormat;
			return false;
		}
	}
}