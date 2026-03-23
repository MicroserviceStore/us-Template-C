# Embedded Microservice Code Generation

## What is an Embedded Microservice?
A platform-agnostic, self-contained binary. Hardware, RTOS, and middleware are abstracted away. Each microservice implements one discrete function. The AI generates only the code strictly needed; no generic libraries.

## Generic Rules

- On receiving a user prompt (e.g. "generate a SHA512 Microservice"), decide the operations, inputs, outputs, and generate all 8 `.INC` files completely.
- C standard is passed in the user prompt; do not assume one.
- Single-threaded: the microservice sleeps until a command arrives and responds one at a time. No thread-safety needed.
- Do not use any standard library function (e.g. `memcpy`, `memset`, `memcmp`). Implement all such operations inline using loops.
- No dynamic memory (`malloc`/`calloc`/`free`).
- Default endianness: Little-Endian unless stated otherwise.
- No magic numbers, use `#define` macros (e.g. `SHA512_BUFFER_MAX_SIZE`).
- Consider memory-constraint microcontrollers for memory usage in code generation.
- Target 32-bit MCUs. Avoid `uint64_t` unless the algorithm specification mandates it (e.g. SHA-512 state words). Use `uint32_t` for counters, lengths, and sizes.
- Struct sizes in `us_operation_inputs.inc` and `us_operation_outputs.inc` + uServicePackageHeader must not exceed 256 bytes unpacked. If buffers would exceed this, reduce them to fit and make large buffers a multiple of 16. Document the max allowed size in the function header.
- Never add C header include guards (`#ifndef`/`#define`/`#endif`).
- API pointer args are copied by value into the serialised input/output structs, never pass by reference into the microservice.
- `SysStatus` (runtime level) is handled by the template; do not define it.
- `usRequestPackage` / `usResponsePackage` are defined in the template as:
    ```c
    typedef struct { int16_t operation; int16_t status; uint16_t length; uint16_t __reserved; } uServicePackageHeader;
    typedef struct { uServicePackageHeader header; union { #include "us_operation_inputs.inc" } payload; } usRequestPackage;
    typedef struct { uServicePackageHeader header; union { #include "us_operation_outputs.inc" } payload; } usResponsePackage;
    typedef struct { uServicePackageHeader header; uint8_t payload[1]; } uServicePackage;
    ```
- Comments must describe only what the code does. Do not mention the template project, other `.INC` files, or any code generation rules.

## Files to Generate

### `us_operations.inc`
C enum values (no `typedef`, no braces, this is `#include`d inside an existing enum).
Naming: `usOP_` prefix. Example: `usOP_sha512,`
Macros shared across all files should also be defined here (first file included).

### `us_api.inc`
Public API header. 

Always include:
```c
SysStatus us_{MicroserviceName}_Initialise(void); // template implements this
```

Then one function per operation:
- Name: `us_{MicroserviceName}_{Operation}`
- Returns `SysStatus`
- Has `usStatus* status` output param
- Doxygen headers
- Example: `SysStatus us_SHA256_Calculate(uint8_t* buf, uint32_t len, uint8_t hash[32], uint32_t timeoutInMs, usStatus* status);`

Predefined `usStatus` values (template-provided, do not redefine):
`usStatus_Success=0`, `usStatus_InvalidOperation`, `usStatus_Timeout`, `usStatus_NoSessionSlotAvailable`, `usStatus_InvalidSession`, `usStatus_InvalidParam`, `usStatus_InvalidParam_UnsufficientSize`, `usStatus_InvalidParam_SizeExceedAllowed`. Custom codes start at `usStatus_CustomStart=32`.

File Rules:
- Every API function returns even if there is no output, always return `SysStatus_Success` and `usStatus_Success`.
- `us_api.inc` is included in caller code with no prior includes. Redefine all macros required by API function signatures (e.g. buffer size constants) directly in `us_api.inc`, do not rely on macros defined in other `.INC` files.

### `us_operation_inputs.inc`
Anonymous structs included into `usRequestPackage::payload` union. One per operation.

Example:
```c
struct { uint8_t buffer[128]; uint32_t len; } sha256;
```

### `us_operation_outputs.inc`
Same pattern as inputs but for `usResponsePackage::payload`. Example:
```c
struct { uint8_t hash[32]; } sha256;
```

### `us_userlib.inc`
Implements each API function from `us_api.inc`. Packs the request, calls `uService_RequestBlocker`, unpacks the response. 

```c
// Signature (template-provided):
SysStatus uService_RequestBlocker(uint32_t uServiceHandle, uServicePackage* req, uServicePackage* resp, uint32_t timeoutInMs);

// Call as:
retVal = uService_RequestBlocker(userLibSettings.execIndex, (uServicePackage*)&request, (uServicePackage*)&response, timeoutInMs);
```

File Rules:
- Do not implement `us_{MicroserviceName}_Initialise()` in `us_userlib.inc`, template provides it.
- Functions in us_userlib.inc shall define `SysStatus retVal;`
- header.length of request packages shall be the header(uServicePackageHeader) + payload size.
- Copy response outputs with explicit sizes (array parameters decay to pointers; never use `sizeof(param)` for copy length).
- Do not check for NULL pointers; memory isolation handles invalid pointers.
- Do not set the __reserved field in the request package header.

### `operation_func.inc`
Implements the actual algorithm(s).

Entry point per operation:
```c
usStatus sha256(uint8_t* buffer, uint32_t len, uint8_t* hash) { ... return usStatus_Success; }
```

File Rules:
- May use `static` helper functions, `static` local buffers (safe: single-threaded), and `#define` macros. All declared in this file.

### `us_operation_parser.inc`
`case` entries for a `switch` on the operation ID. Calls the function from `operation_func.inc`, sets `response.header.length`, calls `Sys_SendMessage` to response. 

File Rules:
- `retVal` and `userLibSettings` are provided by template. 
- Respond via `Sys_SendMessage(uint8_t destinationID, uint8_t* message, uint32_t len, uint32_t* sequenceNo)` in `us_operation_parser.inc`. Template provides `destinationID`, `sequenceNo`; only initialise `response`. Set retVal to return value of `Sys_SendMessage()`
- Do not check return values in us_operation_parser.inc.

Example:
```c
case usOP_sha256:
    response.header.status = (int16_t)sha256(request->payload.sha256.buffer, request->payload.sha256.len, response.payload.sha256.hash);
    response.header.length = (uint16_t)sizeof(response.payload.sha256);
    retVal = Sys_SendMessage(senderID, (uint8_t*)&response, sizeof(usResponsePackage), &sequenceNo);
    break;
```

### `us_operation_test.inc`
One or more scoped test blocks per API function.

Example:
```c
{
    usStatus status;
    uint8_t buf[] = "abc";
    uint8_t calculated[32];
    static const uint8_t expected[32] = { 0xba,0x78,...,0xad };
    retVal = us_SHA256_Calculate(buf, sizeof(buf)-1, calculated, 1000u, &status);
    CHECK_US_ERR(retVal, status);
    LOG_INFO("SHA256 Test: %s", memcmp(calculated, expected, 32)==0 ? "Success" : "Failed");
}
```

File Rules:
- Use `CHECK_US_ERR(SysStatus, usStatus)` and `LOG_INFO(char* format, ...)` provided by template.
- Include positive, negative and edge cases: empty input, max-size input, oversized input (expect `usStatus_InvalidParam_SizeExceedAllowed`).
- `retVal` are provided by template.
- Use approved test data sets. For example for crypto, FIPS/standard known-answer vectors where available.

### `AICodeGenerationNotes.txt`
Plain-text short bullet lists covering: algorithm design decisions, buffer size calculations, any deviations from default rules, warnings (e.g. truncated output, token limits), and anything the developer should know before integrating the files.

## Output Format

Wrap every generated file in XML tags. Output nothing outside these tags.

<file name="us_operations.inc">
/* content */
</file>

<file name="us_api.inc">
/* content */
</file>

<file name="us_operation_inputs.inc">
/* content */
</file>

<file name="us_operation_outputs.inc">
/* content */
</file>

<file name="us_userlib.inc">
/* content */
</file>

<file name="operation_func.inc">
/* content */
</file>

<file name="us_operation_parser.inc">
/* content */
</file>

<file name="us_operation_test.inc">
/* content */
</file>

<file name="AICodeGenerationNotes.txt">
/* content */
</file>
