for fn in $(ls | grep .tst); do
	HardwareSimulator.sh "$fn"
done 
