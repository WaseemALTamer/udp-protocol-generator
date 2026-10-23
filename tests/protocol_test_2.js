import {
    Message, 
    Command, 
    WifiConnect, 
    BytesStream,
    ContentType
} from "./protocol.js";






// Quick encode/decode test with nested command and byte stream
const m = new Message();

const s = new BytesStream();

s.content = new Uint8Array([
    33, 44, 44, 10, 10, 10, 10, 255, 52, 33,
    0, 0, 0, 0, 0, 0, 0, 10, 10, 10, 10, 255, 52, 33,
    10, 10, 10, 10, 255, 52, 33,
    10, 10, 10, 10, 255, 52, 33,
    10, 10, 10, 10, 255, 52, 33,
    10, 10, 10, 10, 255, 52, 33,
    52, 33, 10, 10, 10, 10, 255, 52, 33,
    52, 33, 10, 10, 10, 10, 255, 52, 33,
]);

m.content_type = ContentType.BYTES_STREAM;
m.content = s;

// Encode
const encoded = m.to_bytes();

console.log("Encoded:", encoded);
console.log("Encoded length:", encoded.length);

// Decode
const m2 = new Message();
m2.decode(encoded);

console.log("Decoded message:", m2);
console.log("Content type:", m2.content_type);
console.log("Stream:", m2.content);

