# Change Log
All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [1.0.0] - 2016-03-07
### Changed
- Request class becomes ClientHandler

### Added
- ServerHandler class API, allowing to build a server to listen requests, build and send responses.
- Basic ServerHandler example
- ServoMotor management via a REST API build using ServerHandler

## [0.1.1] - 2016-02-28
### Changed
- init signature, from now it returns a bool.

## 0.1.0 - 2016-02-28
### Added
- README
- GET basic example.
- Basics of the library API
	* Constructor
	* init
	* Methods to handle request = initRequest, addHeader, send
	* Return statusCode and body (const char* format)

[1.0.0]: https://github.com/jomaora/schapiuino/compare/v0.1.1...v1.0.0
[0.1.1]: https://github.com/jomaora/schapiuino/compare/v0.1.0...v0.1.1

