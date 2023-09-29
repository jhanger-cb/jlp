# jlp - C++ based log parser for Jenkins Logs
## Jenkins Log Parser

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

