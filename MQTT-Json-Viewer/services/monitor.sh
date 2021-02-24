#!/bin/sh
#!/bin/bash
Year=`date +%Y`
Month=`date +%m`
Day=`date +%d`
echo "Current Date is: $Day-$Month-$Year"
logfile="/var/log/mmdvm/MMDVM_Bridge-$Year-$Month-$Day.log"
mqtt_server="192.168.x.xxx"
pub_feed="Home/MMDVMBridge/LastHeard"
if [ -f "$logfile" ]; then
echo "Logfile found at"
echo "$logfile "
echo "starting monitoring on logfile"
else
echo "Logfile does not exist, please check MMDVMHost"
fi
tail -fn0 $logfile | while read line ; do
#        echo “$line” | grep 'voice header'
        if [ $? = 0 ]
        then
                # Actions
               echo "$line" | sed -u -n '/voice header/p'| sed -u 's/^.*\(from.*to\).*$/\1/' | sed -u 's/[^ ]* //' | sed -u 's/ to//g' | mosquitto_pub -h $mqtt_server -t $pub_feed -l
	fi
done


