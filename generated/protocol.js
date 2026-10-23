
const VERSION = 2;



export const ContentType = Object.freeze({
    NONE: 0,
    BYTES_STREAM: 1,
    MESSAGE: 2,
    COMMAND: 3,
    INFORMATION: 4,
    DISCOVERY: 5,
    WIFI_CONNECT: 6,
    SET_DEVICE_STATUS: 7,
    SET_DEVICE_NAME: 8,
    SET_DATE_TIME: 9,
});

export const DeviceStatus = Object.freeze({
    NONE: 0,
    BOOTING: 1,
    NORMAL: 2,
    NETWORK_ERROR: 3,
    UNKNOWN_ERROR: 4,
});





const encoder = new TextEncoder();
const decoder = new TextDecoder("utf-8");

function encodeInt(value, size) {
    const bytes = new Uint8Array(size);
    let n = BigInt(value);

    for (let i = 0; i < size; i++) {
        bytes[i] = Number(n & 0xffn);
        n >>= 8n;
    }

    if (n !== 0n) {
        throw new RangeError(`Integer does not fit in ${size} bytes`);
    }

    return bytes;
}

function decodeInt(bytes) {
    let value = 0n;

    for (let i = bytes.length - 1; i >= 0; i--) {
        value = (value << 8n) | BigInt(bytes[i]);
    }

    return bytes.length <= 6 ? Number(value) : value;
}

function encodeFloat(value, size) {
    const bytes = new Uint8Array(size);
    const view = new DataView(bytes.buffer);

    if (size === 4) view.setFloat32(0, value, true);
    else if (size === 8) view.setFloat64(0, value, true);
    else throw new Error(`Invalid float size: ${size}`);

    return bytes;
}

function decodeFloat(bytes) {
    const view = new DataView(
        bytes.buffer,
        bytes.byteOffset,
        bytes.byteLength
    );

    if (bytes.length === 4) return view.getFloat32(0, true);
    if (bytes.length === 8) return view.getFloat64(0, true);

    throw new Error(`Invalid float size: ${bytes.length}`);
}

function encodeString(value, size, name) {
    const bytes = encoder.encode(value);

    if (bytes.length > size) {
        throw new RangeError(
            `Field '${name}' is too long: ${bytes.length} bytes ` +
            `(maximum ${size})`
        );
    }

    const result = new Uint8Array(size);
    result.set(bytes);
    return result;
}

function concatBytes(parts) {
    const length = parts.reduce((sum, part) => sum + part.length, 0);
    const result = new Uint8Array(length);

    let offset = 0;

    for (const part of parts) {
        result.set(part, offset);
        offset += part.length;
    }

    return result;
}

export class MessageBase {
    static fields = {};

    to_bytes() {
        const parts = [];

        for (const [name, [type, size]] of Object.entries(
            this.constructor.fields
        )) {
            const value = this[name];

            if (value == null) continue;

            if (type === "str") {
                parts.push(encodeString(value, size, name));
            } else if (type === "int") {
                parts.push(encodeInt(value, size));
            } else if (type === "float") {
                parts.push(encodeFloat(value, size));
            } else {
                throw new TypeError(`Cannot encode field '${name}'`);
            }
        }

        if (this.content instanceof MessageBase) {
            parts.push(this.content.to_bytes());
        }

        return concatBytes(parts);
    }

    decode(data) {
        const bytes = data instanceof Uint8Array
            ? data
            : new Uint8Array(data);

        let offset = 0;

        for (const [name, [type, size]] of Object.entries(
            this.constructor.fields
        )) {
            if (offset + size > bytes.length) {
                throw new RangeError(`Incomplete field '${name}'`);
            }

            const chunk = bytes.subarray(offset, offset + size);
            offset += size;

            if (type === "str") {
                let end = chunk.length;

                while (end > 0 && chunk[end - 1] === 0) {
                    end--;
                }

                this[name] = decoder.decode(chunk.subarray(0, end));
            } else if (type === "int") {
                this[name] = decodeInt(chunk);
            } else if (type === "float") {
                this[name] = decodeFloat(chunk);
            } else {
                throw new TypeError(`Cannot decode field '${name}'`);
            }
        }

        if (
            "content_type" in this &&
            "content" in this &&
            offset < bytes.length &&
            Number(this.content_type) !== ContentType.NONE
        ) {
            const MessageClass = MESSAGE_TYPE_REGISTRY[
                Number(this.content_type)
            ];

            if (!MessageClass) {
                throw new Error(
                    `Unknown content type: ${this.content_type}`
                );
            }

            this.content = new MessageClass();
            this.content.decode(bytes.subarray(offset));
        }

        return this;
    }
}

export class BytesStream extends MessageBase {
    static size = 4;
    static fields = {
        length: ["int", 4],
    };

    constructor(content = new Uint8Array()) {
        super();
        this.length = 0;
        this.content = content;
    }

    to_bytes() {
        const content = this.content instanceof Uint8Array
            ? this.content
            : new Uint8Array(this.content);

        this.length = content.length;

        return concatBytes([
            encodeInt(this.length, 4),
            content,
        ]);
    }

    decode(data) {
        const bytes = data instanceof Uint8Array
            ? data
            : new Uint8Array(data);

        if (bytes.length < 4) {
            throw new RangeError("Incomplete BytesStream header");
        }

        this.length = Number(decodeInt(bytes.subarray(0, 4)));
        this.content = bytes.slice(4);

        return this;
    }
}

export class Command extends MessageBase {
    static size = 9;

    static fields = {
        sequence_number: ["int", 8],
        content_type: ["int", 1],
    };

    constructor() {
        super();
        this.sequence_number = 0;
        this.content_type = ContentType.NONE;
        this.content = null;
    }
}

export class Discovery extends MessageBase {
    static size = 0;

    static fields = {
    };

    constructor() {
        super();
    }
}

export class Information extends MessageBase {
    static size = 0;

    static fields = {
    };

    constructor() {
        super();
    }
}

export class Message extends MessageBase {
    static size = 80;

    static fields = {
        version: ["int", 1],
        device_name: ["str", 64],
        mac_address: ["int", 6],
        time_stamp: ["float", 8],
        is_encrypted: ["int", 1],
        content_type: ["int", 1],
    };

    constructor() {
        super();
        this.version = 2;
        this.device_name = '';
        this.mac_address = 0;
        this.time_stamp = 0;
        this.is_encrypted = 0;
        this.content_type = ContentType.NONE;
        this.content = null;
    }
}

export class SetDateTime extends MessageBase {
    static size = 6;

    static fields = {
        seconds: ["int", 1],
        minutes: ["int", 1],
        hours: ["int", 1],
        day: ["int", 1],
        month: ["int", 1],
        year: ["int", 4],
    };

    constructor() {
        super();
        this.seconds = 0;
        this.minutes = 0;
        this.hours = 0;
        this.day = 0;
        this.month = 0;
        this.year = 0;
    }
}

export class SetDeviceName extends MessageBase {
    static size = 64;

    static fields = {
        device_name: ["str", 64],
    };

    constructor() {
        super();
        this.device_name = '';
    }
}

export class SetDeviceStatus extends MessageBase {
    static size = 1;

    static fields = {
        status: ["int", 1],
    };

    constructor() {
        super();
        this.status = DeviceStatus.NONE;
    }
}

export class WifiConnect extends MessageBase {
    static size = 96;

    static fields = {
        ssid: ["str", 32],
        password: ["str", 64],
    };

    constructor() {
        super();
        this.ssid = '';
        this.password = '';
    }
}



export const MESSAGE_TYPE_REGISTRY = Object.freeze({
    [ContentType.BYTES_STREAM]: BytesStream,
    [ContentType.COMMAND]: Command,
    [ContentType.DISCOVERY]: Discovery,
    [ContentType.INFORMATION]: Information,
    [ContentType.MESSAGE]: Message,
    [ContentType.SET_DATE_TIME]: SetDateTime,
    [ContentType.SET_DEVICE_NAME]: SetDeviceName,
    [ContentType.SET_DEVICE_STATUS]: SetDeviceStatus,
    [ContentType.WIFI_CONNECT]: WifiConnect,
});
