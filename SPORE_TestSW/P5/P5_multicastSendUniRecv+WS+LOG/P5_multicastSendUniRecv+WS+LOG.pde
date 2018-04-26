import oscP5.*;
import websockets.*;

Table csv;

OscP5 oscMult;
OscP5 osc;
WebsocketServer ws;
int timer;
float x,y;

int id = 0;
float voltage = 0.0f;





void setup() {
  size(300, 300);
  csv = new Table();
  csv.addColumn("id");
  csv.addColumn("voltage");
  csv.addColumn("timestamp");
  
  System.setProperty("java.net.preferIPv4Stack", "true");     // disable ipv6
  //OSC: 
  oscMult = new OscP5(this, "239.0.1.1", 7777);
  osc = new OscP5(this, 8888);
  osc.plug(this, "battery", "/battery");
  //WS: 
  ws= new WebsocketServer(this,8080,"/");
  timer=millis();
  x=0;
  y=0;
  sendBrightness(1.0f);
  sendSendRate(3.0f);
  
}



void draw() {
  background(0);
  ellipse(x,y,10,10);
  if(millis()>timer+5000){              // currently SPORE responds with id+voltage when it receives a message
    ws.sendMessage("Server message");
    timer=millis();
  }
  fill(255);
  text("id: " + id, 10, 20);
  text(nf(voltage, 0, 3) + " volts", 10, 40);
}



void mousePressed() {
  if (mouseButton == LEFT) sendBrightness(map(mouseY, 0, height, 1.0f, 0.0f));
  if (mouseButton == RIGHT) sendSendRate(map(mouseX, 0, width, 0.0f, 30.0f));
}

void sendBrightness(float val) {
  OscMessage m = new OscMessage("/brightness");
  m.add(val);
  oscMult.send(m);
}

// spre's osc send rate:
void sendSendRate(float val) {
  OscMessage m = new OscMessage("/sendRate");
  m.add(val);
  oscMult.send(m);
}


void battery(int addr, float volt) {
  println("device #" + addr + " reporting " + nf(volt, 0, 3) + " volts");
}

/* incoming osc message are forwarded to the oscEvent method. */
void oscEvent(OscMessage theOscMessage) {
  if (theOscMessage.isPlugged()==false) {
    /* print the address pattern and the typetag of the received OscMessage */
    print("### received an osc message.");
    print(" addrpattern: "+theOscMessage.addrPattern());
    println(" typetag: "+theOscMessage.typetag());
    //println(theOscMessage.get(0).floatValue());
    //println(theOscMessage.get(1).floatValue());
  }
}

void webSocketServerEvent(String msg){
 println(msg);
 x=random(width);
 y=random(height);
 JSONObject json = parseJSONObject(msg);
 if (json != null) {
   id = json.getInt("id");
   voltage = json.getFloat("battery");
   TableRow row = csv.addRow();
   row.setInt("id", id);
   row.setFloat("voltage", voltage);
   int seconds = (int) (millis() / 1000) % 60 ;
   int minutes = (int) ((millis() / (1000*60)) % 60);
   int hours   = (int) ((millis() / (1000*60*60)) % 24);
   row.setString("timestamp", nf(hours,2) +  ":" + nf(minutes,2) + ":" + nf(seconds,2));
   
   
   saveTable(csv, "data/sporeBattery.csv");
 }
}
