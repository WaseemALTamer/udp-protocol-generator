from pathlib import Path
BASE_DIR = Path(__file__).resolve().parent




import protocol



from dataclasses import is_dataclass, fields, MISSING
from enum import Enum
import inspect
import os



BASE_DIR = Path(__file__).resolve().parent


def generate_message_class(cls) -> str:
    """Generate a JavaScript message class from a Python dataclass."""

    lines = [
        f"export class {cls.__name__} extends MessageBase {{",
        f"    static size = {getattr(cls, '__size__', 0)};",
        "",
        "    static fields = {",
    ]

    for name, (field_type, size) in cls.__fields__.items():
        js_type = {
            str: "str",
            int: "int",
            float: "float",
        }.get(field_type)

        if js_type is None:
            raise TypeError(
                f"Unsupported field type {field_type!r} "
                f"for {cls.__name__}.{name}"
            )

        lines.append(f'        {name}: ["{js_type}", {size}],')

    lines.extend([
        "    };",
        "",
        "    constructor() {",
        "        super();",
    ])

    for field in fields(cls):
        name = field.name

        if name == "content":
            lines.append("        this.content = null;")
            continue

        if field.default is not MISSING:
            value = field.default

            if isinstance(value, Enum):
                value = f"{type(value).__name__}.{value.name}"
            elif isinstance(value, str):
                value = repr(value)
            elif value is None:
                value = "null"
            elif isinstance(value, bool):
                value = str(value).lower()
            else:
                value = str(value)

            lines.append(f"        this.{name} = {value};")

        elif field.default_factory is not MISSING:
            # Do not execute Python default factories.
            lines.append(f"        this.{name} = 0;")

        else:
            lines.append(f"        this.{name} = null;")

    lines.extend([
        "    }",
        "}",
    ])

    return "\n".join(lines)

def generate_enum(cls) -> str:
    """Generate a JavaScript enum object from a Python Enum."""

    lines = [
        f"export const {cls.__name__} = Object.freeze({{",
    ]

    for member in cls:
        lines.append(
            f"    {member.name}: {member.value},"
        )

    lines.extend([
        "});",
    ])

    return "\n".join(lines)


def generate_registry(message_classes) -> str:
    """Generate the content-type to message-class registry."""

    lines = [
        "export const MESSAGE_TYPE_REGISTRY = Object.freeze({",
    ]

    for cls in message_classes:
        # Only register classes whose names correspond to ContentType members.
        if cls.__name__ == "Message":
            content_type = "MESSAGE"
        elif cls.__name__ == "BytesStream":
            content_type = "BYTES_STREAM"
        else:
            content_type = cls.__name__[
                0:1
            ] + cls.__name__[1:]

            # Convert class names such as WifiConnect to WIFI_CONNECT.
            content_type = "".join(
                "_" + char if char.isupper() else char
                for char in cls.__name__
            ).upper().lstrip("_")

        lines.append(
            f"    [ContentType.{content_type}]: {cls.__name__},"
        )

    lines.extend([
        "});",
    ])

    return "\n".join(lines)





generated_js = """
/*
    THIS CODE IS GENEARTED DO NOT CHANGE, IF YOU WANT TO CHANGE LOOK THROUGH THE
    PYTHON MIRROR OF THE PROTOCAL AND REGENERATE THE CODE WITH THE GENERATOR
*/




const VERSION = {PROTOCOL_VERSION};



{ENUMS_GENERATED}



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

{MESSAGES_CLASSES_GENERATED}

{MESSAGE_TYPE_REGISTRY_GENERATED}
"""



generated_js = generated_js.replace("{PROTOCOL_VERSION}", str(protocol._version))




enums = []
message_classes = []

for name, cls in inspect.getmembers(protocol, inspect.isclass):
    if cls.__module__ != protocol.__name__:
        continue

    if cls is protocol.MessageBase:
        continue

    if (
        is_dataclass(cls)
        and issubclass(cls, protocol.MessageBase)
    ):
        message_classes.append(cls)

    elif issubclass(cls, Enum):
        enums.append(cls)




data = ""

for enum_cls in enums:
    data += generate_enum(enum_cls) + "\n\n"

generated_js = generated_js.replace(
    "{ENUMS_GENERATED}",
    data
)


data = ""

for message_cls in message_classes:

    if message_cls is protocol.BytesStream:
        continue

    data += generate_message_class(message_cls) + "\n\n"

generated_js = generated_js.replace(
    "{MESSAGES_CLASSES_GENERATED}",
    data
)


generated_js = generated_js.replace(
    "{MESSAGE_TYPE_REGISTRY_GENERATED}",
    generate_registry(message_classes)
)


if __name__ == "__main__":

    folder = "generated"
    os.makedirs(folder, exist_ok=True)

    js_file_path = os.path.join(folder, "protocol.js")


    with open(js_file_path, "w") as f:
        f.write(generated_js)