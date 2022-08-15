var quantity = topic.substr(topic.length - 4);
switch(quantity)
{
	case "pres": 
	msg.payload = msg.payload / 1000; // covert Pa to kPa
	break
}
return msg