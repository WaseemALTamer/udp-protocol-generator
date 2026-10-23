import {
    Message,
    Command,
    WifiConnect,
    BytesStream,
    ContentType
} from "./protocol.js";

// Create the outer message
const m = new Message();

m.version = 2;
m.device_name = "ESP32_Device";
m.mac_address = 0x123456789ABC;
m.time_stamp = 1720000000.5;
m.is_encrypted = 0;

// Create a command containing a WiFi connection message
const command = new Command();
command.sequence_number = 42;
command.content_type = ContentType.WIFI_CONNECT;

const wifi = new WifiConnect();
wifi.ssid = "MyNetwork";
wifi.password = "SuperSecret123";

command.content = wifi;

// Set the nested message
m.content_type = ContentType.COMMAND;
m.content = command;

// Create the separate trailing byte stream
const stream = new BytesStream();
stream.content = new Uint8Array([
    0x10, 0x20, 0x30, 0x40,
    0x50, 0x60, 0x70, 0x80,
]);

// Encode the nested message recursively, then append the stream
const messageBytes = m.to_bytes();
const streamBytes = stream.to_bytes();

const encoded = new Uint8Array(
    messageBytes.length + streamBytes.length
);

encoded.set(messageBytes, 0);
encoded.set(streamBytes, messageBytes.length);

console.log("Encoded length:", encoded.length);
console.log("Message length:", messageBytes.length);
console.log("Stream length:", streamBytes.length);

// Decode the nested message
const m2 = new Message();
m2.decode(encoded.subarray(0, messageBytes.length));

console.log("Decoded message:", m2);
console.log("Device:", m2.device_name);
console.log("MAC:", m2.mac_address);
console.log("Command sequence:", m2.content.sequence_number);
console.log("WiFi SSID:", m2.content.content.ssid);
console.log("WiFi password:", m2.content.content.password);

// Decode the trailing stream separately
const streamData = encoded.subarray(messageBytes.length);

const stream2 = new BytesStream();
stream2.decode(streamData);

console.log("Stream length:", stream2.length);
console.log("Stream content:", stream2.content);