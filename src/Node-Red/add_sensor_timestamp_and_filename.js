// read sensor and quantity from the topic
var topic = msg.topic;
var info = topic.split('/');
var sensor = info[1];
var quantity = info[2];

if((quantity != "chan") && (quantity != "count") && (quantity != "rssi"))
{
    // add date to the filename
    d = new Date();
    month = d.getMonth() + 1;
    msg.payload = String(Date.now() +"," + msg.payload);
    msg.filename = String("/home/pi/Documents/Node-RED/OutputFiles/S"+ sensor + "_" + quantity + "_" + d.getFullYear() + "-" + month + "-" + d.getDate() + ".txt");
    return msg
}
