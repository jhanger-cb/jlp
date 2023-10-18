#!/usr/bin/bash

filename="jenkins.log.bak"

jlp -asp "${filename}" | egrep 'Lines:|Entries|Processed|Duration'

tsStart=$(date +%s);
wc -l "${filename}"
echo "ALL: " $(awk '{if ($4=="ALL"){print}}' "${filename}" | wc -l) 
echo "DEBUG: " $(awk '{if ($4=="DEBUG"){print}}' "${filename}" | wc -l) 
echo "ERROR: " $(awk '{if ($4=="ERROR"){print}}' "${filename}" | wc -l) 
echo "FATAL: " $(awk '{if ($4=="FATAL"){print}}' "${filename}" | wc -l) 
echo "FINE: " $(awk '{if ($4=="FINE"){print}}' "${filename}" | wc -l)
echo "FINER: " $(awk '{if ($4=="FINER"){print}}' "${filename}" | wc -l)
echo "FINEST: " $(awk '{if ($4=="FINEST"){print}}' "${filename}" | wc -l)
echo "INFO: " $(awk '{if ($4=="INFO"){print}}' "${filename}" | wc -l) 
echo "OFF: " $(awk '{if ($4=="OFF"){print}}' "${filename}" | wc -l) 
echo "SEVERE: " $(awk '{if ($4=="SEVERE"){print}}' "${filename}" | wc -l) 
echo "TRACE: " $(awk '{if ($4=="TRACE"){print}}' "${filename}" | wc -l) 
echo "WARN: " $(awk '{if ($4=="WARN"){print}}' "${filename}" | wc -l) 
tsEnd=$(date +%s);

echo "Duration of awks: " $(($tsEnd-$tsStart)) "seconds" 
