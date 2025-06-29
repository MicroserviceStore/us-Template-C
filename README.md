# Embedded Microservice Template Project for C Programming Language

## 1. Overview

Creating a Microservice is a quite lightweight process. Developer is free to use any toolchain, development environment and programming language.

This repository is just a helper template repository that helps developers to create their custom Microservices.

You can see the quick steps to create a Microservice using this repository.
1. [Download the Development Package](#21-download-microservice-package)
2. [Configure the Microservice](#22-configure-the-microservice)
3. [Build the Microservice ](#26-build)
4. [Get Deployment/Output Files ](#27-output--deployment-files)

## 2. Create a Microservice
Microservice Development is a lightweight process with some minor prerequisites.

### 2.1. Download Microservice Package
Please login into your Microservice Dashboard, and select the CPU Processor you would like to work with, and download the package.

> Copy the package to \"**Environment/CPU/**\" directory.

For example: Arm Cortex-M4 Microcontrollers

```C
Environment
├─ CPU
│  ├─ CortexM4
│  ├─ ...
```

### 2.2. Configure the Microservice
Please create a config file under **Configurations/** directory. 

A Microservice can have different versions with different configurations, and these configuration files are used to differentiate the builds.

> * The configuration file extension will be **"*.config"**.

Fields to set;

| Field                            | Description |
|----------------------------------|-------------|
| **uSERVICE_NAME**               | Max 15-character Microservice Name to identify the Microservice. |
| **uSERVICE_VERSION_STR**        | Microservice Version in String Format. <Major>.<Minor> |
| **uSERVICE_MAINSTACK_SIZE**     | Main Stack Size of the Microservice. Only applies to the main thread; developers may create child threads at runtime with custom sizes. |
| **uSERVICE_CPU_CORE**           | Target CPU Core for the Microservice build. Must match the CPU Core selected during Microservice Package Download (see `Environment/CPU/<CPUCORE>`). |
| **uSERVICE_CFLAGS**             | Compile-time flags for the Microservice (CFLAGS). |
| **uSERVICE_LDFLAGS**            | Link-time flags for the Microservice (LDFLAGS). |
| **uSERVICE_SOURCE_FILES**       | Source files used to build the Microservice. |
| **uSERVICE_INCLUDE_DIRS**       | Include directories used during the build. |

Toolchain-Specific Flags 
(GCC)

| Field                            | Description |
|----------------------------------|-------------|
| **uSERVICE_TOOLCHAIN_GCC_PATH** | Path to the GCC toolchain. Developers can use any toolchain, and must specify the path. |
| **uSERVICE_GCC_LD_PATH**        | [Optional] Path to the linker script (GCC-specific). |

Please see an example hello world example below, name MyService.config, for CortexM4

```C
uSERVICE_NAME = MyService
uSERVICE_VERSION_STR = "0.9"
uSERVICE_MAINSTACK_SIZE = 0x800

uSERVICE_CPU_CORE = CortexM4

uSERVICE_CFLAGS = -O2
uSERVICE_LDFLAGS = \
uSERVICE_SOURCE_FILES = Source/main.c
uSERVICE_INCLUDE_DIRS = -IInclude/

uSERVICE_TOOLCHAIN_GCC_PATH="GNU Arm Embedded Toolchain/10 2021.10/bin/"

uSERVICE_GCC_LD_PATH=microservice.ld
```

And to run this configuration
> make CONFIG=MyService

### 2.3. Linker Scripts
Microservice Runtime shall have some sections and shall expose some symbols.

✅ All these already handled using Environment/Toolchain/\<TOOLCHAIN\>/microservice_template.ld so there is no need to do anything by the developer.

> * Developer can use this file or can create a new copy, and point this .ld

> * The developer is free to modify and additional sections and symbols.

See [Generic Microservice Requirements](Documentation/MicroserviceRequirements.md).

### 2.4. Required Symbols by Microservice Runtime
Microservice Runtime expects some symbols from a Microservice. 

✅ All these already handled using Environment/Toolchain/\<TOOLCHAIN\>/vector.s so there is no need to do anything by the developer.

See [Required Symbols](Documentation/MicroserviceRequirements.md#required-symbols) in "Generic Microservice Requirements" for more details.

### 2.5. Microservice API
Each Microservice needs to provide an interface for the callers. It will be a package by the Microservice Vendor as part of the Microservice deployment.

i. A static library to keep the custom Microservice API
ii. Header Files

This part is detailed in the [Microservice Implementation & Interface](#3-microservice-implementation-and-interface) section.

### 2.6. Build
There are various rules to build and get entities. And entities can be generated for different configurations by passing a \<CONFIG_NAME\> argument. 

\<CONFIG_NAME\> must be a file under **Configuration/** directory with **"*.config"** extension.

1. **"default" or "microservice"**: Build the Microservice and generates the Microservice executable.

    > make CONFIG=\<CONFIG_NAME\>

    or

    > make microservice CONFIG=\<CONFIG_NAME\>

    
    Microservice Executable is generated under Output/\<uSERVICE_CPU_CORE\>/\<TOOLCHAIN\>\Image directory. There will be two version; \<uSERVICE_NAME\>.bin (binary format) and \<uSERVICE_NAME\>.hex (intel-hex format).

2. **"userlib"**: Build the User Lib of Microservice and generates the static libraries and header files for caller executables to interact with Microservice.
The developer can build the Microservice for various configurations.

    > make userlib CONFIG=\<CONFIG_NAME\>

    Microservice Executable is generated under Output/\<uSERVICE_CPU_CORE\>/\<TOOLCHAIN\>\UserLib folder.

    \> UserLib is generated under Output/\<uSERVICE_CPU_CORE\>/\<TOOLCHAIN\>\UserLib directory. There will be a static file(libuS-\<uSERVICE_NAME\>UserLib_\<TOOLCHAIN\>.a) and the public header (us-\<uSERVICE_NAME\>.h) of the static library.

3. **"package"**: Generates the whole deployment package for Microservice Store to submit.

    i. Generates Microservice Executable

    ii. Generates User Lib and API Headers

    iii. Generates a ZIP file to submit to Microservice Store

    > make package CONFIG=\<CONFIG_NAME\>

### 2.7. Output & Deployment Files
Outputs files are collected under **Output/\<uSERVICE_CPUCORE\>/\<TOOLCHAIN\>/** directory.

The developer can use Image Outputs to submit the Microservices to the Microservice Store under **Output/\<uSERVICE_CPUCORE\>/\<TOOLCHAIN\>/Image**

> * **\<uSERVICE_NAME\>.bin** : Microservice Binary Output
> * **\<uSERVICE_NAME\>.hex** : Microservice IntelHex Output

#### 2.7.1 Helper Outputs
The developer can get more details about the Microservice for debug purposes.

> * **\<uSERVICE_NAME\>.elf** : ELF Output
> * **\<uSERVICE_NAME\>.elf.map** : MAP Output

> * **ImageDetails/\<uSERVICE_NAME\>.objdump** : Object dump
> * **ImageDetails/\<uSERVICE_NAME\>.size** : Symbols and Symbol Sizes
> * **ImageDetails/\<uSERVICE_NAME\>.sym** : Symbols

## 3. Microservice Implementation and Interface

> \> **IMPORTANT**: Before starting, the development environment can be challenging for developers to develop a portable and platform-agnostic microservice. Herein, the developer can use Simulation Environments that does not need hardware. See [Microservice Simulation Development Implementation](#34-microservice-simulation-environment) section.

A Microservice basically is built by two parts

i. Microservice Executable: A self-container independent and isolated execution. See [Microservice Implementation](#33-microservice-implementation)

ii. An Interface, such as a static library and Header files, that run in the caller executions to interact with the Microservice. See [Microservice API](#32-creating-microservice-api)

### 3.1. Internal Definitions

> \> **RECOMMENDATION**: Create a us_Internal.h under Include/ directory for internal definitions, where the build system would use it for Package Generations. There is an template header under Include/ directory called "us_Internal.h"; you can modify and use it.

It is recommended to define internal definitions, such as request and response packages structures, to share between Microservice and the User APIs called from Caller Executions.

A Microservice Request and Response packages shares the same structure : HEADER (8-Bytes) + Payload.

See the header structure below

| Field            | Size |Description |
|------------------|------|------------|
| **Operation ID** | 16-Bit | Operation ID to parse the package |
| **Status** | 16-Bit | Status of the custom operation. Mostly used for response packages. |
| **Length** | 16-Bit | Length of the payload. |
| __reserved | 16-Bit | Reserved |

These structures are defined in "uService.h", and can be used to create the custom request/response structures.

```C
typedef struct
{
    /* uService Operation ID */
    int16_t operation;
    
    /* uService Operation Status */
    int16_t status;

    /* The package length */
    uint16_t length;
    
    uint16_t __reserved;
} uServicePackageHeader;

/* A template struct to show, header + payload offset */
typedef struct
{
    uServicePackageHeader header;
    
    uint8_t payload[1];
} uServicePackage;
```

Example; A microservice with Operating A,B an C, and operation C, needs to send two inputs like 32-bit integer and 8-Bytes Array, and returns 32-bit integer;

```C
typedef struct
{
    usInternalOp_A,
    usInternalOp_B,
    usInternalOp_C,
} usInternalOp;

...

typedef struct
{
    uServicePackageHeader header;

    struct
    {
        uint32_t arg0;
        uint8_t buffer[8];
    } payload;
} usInternalOp_C_Request;

typedef struct
{
    uServicePackageHeader header;

    struct
    {
        uint32_t retval;
    } payload;
} usInternalOp_C_Response;
```

Now both Microservice and the UserLib can share the request and response for the Operation_C.

There is a template header file under Include/ directory. us_Internal.h; developer is free to modify and implement this header.

### 3.2 Creating Microservice API

> \> **RECOMMENDATION**: Create a us-\<uSERVICE_NAME\>.h under Include/ directory as public header, where the build system would use it for Package Generations. There is an template header under Include/ directory called "us-Template.h"; you can rename and use it.

Each microservice shall provide an API for callers to receive and answer the requests.

> Microservice API runs in the Caller Executable and allows interaction with Microservice using the [Microservice User Lib](#311-creating-a-static-library)

This API shall have
i. Microservice API Functions
ii. Microservice Specific Status/Error Definitions.
iii. Microservice Specific Public Type Definitions

There are some Mandatory and Optional APIs in addition to the Microservice Specific Features.

| API                                 | Mandatory   | Description |
|-------------------------------------|-------------|-------------|
| uS_\<uSERVICE_NAME\>_Initialise()   | Yes         | The caller must call this function to use the Microservice |
| uS_\<uSERVICE_NAME\>_OpenSession()  | Optional    | Open a session in Microservice to not mix or limit the requests.|
| uS_\<uSERVICE_NAME\>_CloseSession() | Optional    | Close a session which opened by OpenSession().|

And then, the developer is free to add any function to the API.

> There is a template header under Include/ directory, called us_Template.h; the developer shall rename it the Microservice name, and define all the public API there. The recommended file notation is us-<uSERVICE_NAME>.h

Each uService API shall return SysStatus type that shows the status of the interaction with the Microservice, such Success, or MicroserviceNotFound, InvalidAccessRight etc, but Microservice shall also define its custom statuses in us-<uSERVICE_NAME>.h
And it is recommended to return microservice specific status as output parameter. 

```C
/* Microservice specific status definition */
typedef enum
{
    us_<uSERVICE_NAME>Status_Success,
    us_<uSERVICE_NAME>Status_UnsupportedOperation,
    ...
} us_<uSERVICE_NAME>Status;

...

SysStatus uS_<uSERVICE_NAME>_OperationC(/* Argument List */, us<uSERVICE_NAME>Status* usStatus);
/* Check both function return and usStatus */

```

#### 3.2.1 Creating a Static Library

> \> **RECOMMENDATION**: There is a source file called UserLib.c under Source/UserLib/ directory, where the build system would use it to generate the API Static Library. The developer can use this file to implement the Microservice API.

The developer shall implement a static library for caller executions that caller executions can use this library to interact with the Microservice.

uS_\<uSERVICE_NAME\>_Initialise() implementation is special, and the developer can use the below implementation directly.
```C
SysStatus us_\<uSERVICE_NAME\>_Initialise(void)
{
    return uService_Initialise(USERVICE_NAME, &userLibSettings.execIndex);
}
```

For the all API functions shall prepare a proper request package, and shall call uService_RequestBlocker() function to interact with the Microservice.

```C

SysStatus us_\<uSERVICE_NAME\>_OperationC(
        /* Arg List*/, 
        \<uSERVICE_NAME\>_Status* status)
{
    usOPCRequestPackage request = { /* Initialise, OP */}
    usOPCResponsePackage response;

    retVal = uService_RequestBlocker(
        /* The index we got in us_\<uSERVICE_NAME\>_Initialise() */
        userLibSettings.execIndex,
        /* Request Package */
        (uServicePackage*)&request,
        /* Response Package */
        (uServicePackage*)&response,
        /* Timeout for the blocker operations */
        timeoutInMs);
    
    if (retVal == SysStatus_Success)
    {
        /*
         * Interaction successful but Microservice may have
         * custom status reporting
         */
        *status = response.header.status;

        /* Set all output parameters using response.payload */
    }

    return retVal;
}

``` 

### 3.3 Microservice Implementation
Microservice is a self-container, isolated executable and process the requests from other executables.

A Microservice implements the following steps
1. [Optional] Calls uService_PrintIntro() to print the its name and version. Developer is free to print anything.
2. Initialise a MessageBox to receive messages from other executions. MessageBox capacity also defines the maximum number of the sessions a Microservice Accept. 
```C
SYS_INITIALISE_IPC_MESSAGEBOX(retVal, <CFG_MAX_NUM_OF_SESSION>);
```
3. Start the Microservice, see below.

A Microservice can sleep until receive a request to not waste the CPU time and power.
So a Microservice can be implemented in any way, multi-threaded or a super loop.

Herein, see an example with a basic Super-Loop implementation.

```C
void startService(void)
{
    while (true)
    {
        bool dataReceived = false;
        int receivedLen = 0;
        
        /* Sleep until receive an IPC message */
        Sys_WaitForEvent(SysEvent_IPCMessage);

        (void)Sys_IsMessageReceived(&dataReceived, &receivedLen, &sequenceNo);       
        if (!dataReceived || receivedLen == 0)
        {
            continue;
        }

        /*
         * TODO : Check for "receivedLen" if enough or will
         * exceed the receive buffer?
         */

        /* Get the message */
        (void)Sys_ReceiveMessage(&senderID, (uint8_t*)&request, 
                                 receivedLen, &sequenceNo);

        /* Process the request */
        processRequest(senderID, &request);
    }
}

SysStatus processRequest(uint8_t senderID, usRequestPackage* request)
{
    switch (request->header.operation)
    {
        case /**/:
            /* 
             * 1. Process the payload
             * 2. Prepare Request
             * 3. Send response to the Sender using senderID
             */             
            Sys_SendMessage(senderID, (uint8_t*)&response,
                            sizeof(usResponsePackage),
                            &sequenceNo);
            break;
        default:
            break;
    }
}

```

### 3.4 Microservice Simulation Environment
A microservice shall support different environments, CPU processors but the developer shall keep in mind a Microservice shall be portable and environment(toolchain/CPU etc) agnostic.

Herein, Microservice Store provides simulation environments for developers to develop their Microservices with no hardware need.

#### 3.4.1 Microservice Windows Form Simulation

Windows Forms allow developers to simulate the hardware environments using the Windows Form to interact with the Microservice or Microservice caller User Applications, such as simulating an LCD with a text box or a physical button with a Form Button.

Windows Forms also offers likely unlimited debugging capabilities, unlimited breakpoints and accessible low level resource simulations.

To run the Windows Simulation
1. Download the "x86" package to Environment/CPU/

2. There are two Visual Studio Project that needs to run parallel, like Microservices on Microcontrollers. Please run **Environment/Simulator/Windows/uServiceSimulator/usSimulator.sln** and **Environment/Simulator/Windows/UserAppSimulator/UserAppSimulator.sln**

3. Configure the Simulation Projects
<br>
See See [Configure Simulations](Environment\Simulator\Windows\README.md).

4. You will see 
    > \> us Test Success

    This is an example Microservice that has one API that sums two integer.

    ```C
    int main(void)
    {
        SysStatus retVal;

        LOG_PRINTF(" > Container : Microservice Test User App");

        SYS_INITIALISE_IPC_MESSAGEBOX(retVal, 4);

        retVal = us_Template_Initialise();
        CHECK_US_ERR(retVal, 0);

        {
            usStatus usStatus;
            int32_t a = 5;
            int32_t b = 6;
            int32_t expectedResult = a + b;
            int32_t result = 0;

            retVal = us_Template_Sum(a, b, &result, &usStatus);
            CHECK_US_ERR(retVal, usStatus);

            LOG_PRINTF(" > us Test %s",
                result == expectedResult ? "Success" : "Failed");
        }
    }
    ```

    The developer, then, can follow the example to implement its custom functionalities.
