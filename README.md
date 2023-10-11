# jlp - C++ based log parser for Java Logs
## Java Log Parser

### By Default starting with Jenkins typical Log File Formats as this is what this solution stems from: 
###     Need to parse and objectify Java Logs from a JVM, Log4J is used in Jenkins;
###     Thus this will default compatible with that typical and most commonly used logging format; 
###     TODO: Set a Logging format for CD/RO; 

### Log Format Object: 
n: date  time  id logLevel  method  [trace|message]
(n+1): if first field is not date, add to object; 
(n+end): next line, first field is date, terminate at previous point; 

### Inputs: 
filename(s)

### Outputs: 
* Log File(s)
* Database Aggregation

### Methods: 
* Read Line
* isEndOfObject (readLine(++iter))

