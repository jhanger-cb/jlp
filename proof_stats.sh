#!/usr/bin/bash

jlp -asp jenkins.log* | egrep 'Lines:|Entries|Processed|Duration'

tsStart=$(date +%s);
wc -l jenkins.log*
echo "ALL: " $(awk '{if ($4=="ALL"){print}}' jenkins.log* | wc -l) 
echo "DEBUG: " $(awk '{if ($4=="DEBUG"){print}}' jenkins.log* | wc -l) 
echo "ERROR: " $(awk '{if ($4=="ERROR"){print}}' jenkins.log* | wc -l) 
echo "FATAL: " $(awk '{if ($4=="FATAL"){print}}' jenkins.log* | wc -l) 
echo "FINE: " $(awk '{if ($4=="FINE"){print}}' jenkins.log* | wc -l)
echo "FINER: " $(awk '{if ($4=="FINER"){print}}' jenkins.log* | wc -l)
echo "FINEST: " $(awk '{if ($4=="FINEST"){print}}' jenkins.log* | wc -l)
echo "INFO: " $(awk '{if ($4=="INFO"){print}}' jenkins.log* | wc -l) 
echo "OFF: " $(awk '{if ($4=="OFF"){print}}' jenkins.log* | wc -l) 
echo "SEVERE: " $(awk '{if ($4=="SEVERE"){print}}' jenkins.log* | wc -l) 
echo "TRACE: " $(awk '{if ($4=="TRACE"){print}}' jenkins.log* | wc -l) 
echo "WARN: " $(awk '{if ($4=="WARN"){print}}' jenkins.log* | wc -l) 
tsEnd=$(date +%s);

echo "Duration of awks: " $(($tsEnd-$tsStart)) "seconds" 
