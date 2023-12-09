# jlp - C++ based log parser for Java Logs

# Releases: 
  * v0.1    - First Stable Release - 10/19/2023
  * v0.2.0  - Refactored Code Base - Stack Trace Aggregation - 10/20/2023
  * v0.3.0 - Regex compilation order changed to improve performance - previous capped at 250 lines per second, now pushing over 15k lines per second parsed - 10/30/2023


## Java Log Parser (jlp)

### By Default starting with Jenkins typical Log File Formats as this is what this solution stems from: 
* Need to parse and objectify Java Logs from a JVM, Log4J is used in Jenkins;
* Thus this will default compatible with that typical and most commonly used logging format; 
* TODO: Set a Logging format for CD/RO; 

### TODOs:
* Configure a Log parsing format for CD/RO; 
  * Expand the logger types supported, from just Java, make a more broad `ulp` -> the Universal Log Parser; 
* Comparison operator overload to compare two log entry items (since timestamp and proc id can be ignored mostly in a comparision). 
* Create an Object class for Stack Trace Line Items to deconstruct the stack trace into more granular components; 
  * Normalization; 
* Port to Java
  * Native Java app port; 
  * Perhaps interface with JNI as a backend library;
* 

### Log Format Object: 
n: date  time  id logLevel  method  [trace|message]
(n+1): if first field is not date, add to object; 
(n+end): next line, first field is date, terminate at previous point; 

### Inputs: 
filename(s)

### Outputs: 
* stdio (currently only working implementation)
* Log File(s)
* Database Aggregation

### Methods: 
* Read Line
* isEndOfObject (readLine(++iter))
