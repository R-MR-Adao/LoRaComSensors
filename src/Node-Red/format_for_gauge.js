var topic = msg.payload
global.set("showData_Gauge1",topic);

var quantity = topic.substr(topic.length - 4);
switch(quantity)
{
    case "temp": // limits = [0 40]
        msg.ui_control = {"min":0,"max":40,"units":"oC"};
        break
    case "pres": // limits = [98 102]
        msg.ui_control = {"min":98,"max":102,"units":"kPa"};
        break
    case "alt": // limits = [0 400]
        msg.ui_control = {"min":0,"max":400,"units":"kPa"};
        break
    case "/hum": // limits = [0 100]
        msg.ui_control = {"min":0,"max":100,"units":"%"};
        break
    case "/NO2": // limits = [0 300]
        msg.ui_control = {"min":0,"max":300,"units":"%"};
        break
    case "/snd": // limits = [40 100]
        msg.ui_control = {"min":40,"max":100,"units":"dBA"};
        break
}
return msg