
## 1. Simulator Project Settings them in the Visual Studio Project Config

(For now, In the absences of a Config file for Simulator, we set the global definitions in the VS Project Settings.)

i. Run Environment\Simulator\Windows\uServiceSimulator\usSimulator.sln project
ii. Right Click on the Project Name (Or "Project" Menu -> "Properties")
iii. Go to "C/C++" -> "Preprocessor" -> "Preprocessor Definitions"
And Set the values as below

### 1.1. Microservice Simulator Settings. 
> Important: 
>   - In the Microservice Simulator project SIM_EXEC_INDEX must be 0.
>   - The USERVICE_NAME must match with the UniqueID of the Microservice in the Microservice Store.

SIM_EXEC_INDEX=0<br>
SIM_CPUCORE_NAME="CortexM4"<br>
USERVICE_NAME="Template"<br>
USERVICE_VERSION_STR="0.9"<br>

### 1.2 Microservice Test Application Settings. 
> Important: 
>   - In the Test App Simulator project SIM_EXEC_INDEX must be 1.
>   - The USERVICE_NAME must match with the UniqueID of the Microservice in the Microservice Store.

SIM_EXEC_INDEX=1<br>
SIM_CPUCORE_NAME="CortexM4"<br>
USERVICE_NAME_NONSTR=Template<br>


## 2. Please add all the related source files from the Microservice
   IMPORTANT: The simulations are managed(.NET) project with Common Language RunTime (CLR), but the Microservice source files must be build with "No Common Language RunTime" option. Please right click the Microservice Source Files (.c), and go to C/C++ -> General and Select "No Common Language RunTime Support" in "Common Language RunTime Support"

## 3. Please add all the related source files from the Microservice
