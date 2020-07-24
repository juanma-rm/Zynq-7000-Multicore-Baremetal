# Zynq-7000-Multicore-Baremetal

The main goal of this project is to analyse the Zynq-7000 architecture (with a Zybo board) in order to exploit the dual-core (ARM-Cortex A9) architecture in baremetal.

The project is structured in several applications that add features of the architecture step by step, starting from scratch. The flow goes as follows:
  1) "App_1_GPIOAndHelloWorld". Basic test of PS (Processing System) and PL (Programmable Logic): using one ARM core, print helloworld and control a GPIO core implemented in PL to manipulate on-board LED state using Xilinx drivers for GPIO.
  2) "App_2_ARM_Private_Interrupt". Use of ARM-core-private interruptions (single core).
  3) "App_3_Multicore_Simple_Ax". Use both ARM cores to do a first approach of multicore (each core running some application similar to the previous one).
      - Conditions: both cores running independetly, each one running its own code and using its own private memory section.
  4) "App_4_Multicore_SharedInterr_Ax". Use shared / intercore interruptions to communicate ARM both cores.
      - Conditions: both cores running independetly, each one running its own code and using its own private memory section.
  5) "App_5_Multicore_SharedMemory_Ax". Add shared-memory for both cores. This will require them to communicate each other (using interruptions) and to use barriers to guarantee that instructions accessing to shared-memory have finished before of notifying the other core.
      - Conditions: cache is not used.
  6) "App_6_Multicore_SharedMemory_L1_Ax". Similar to App5 (shared-memory for both cores).
      - Conditions: cache L1 used.
  7) "App_7_Multicore_SharedMemory_L2_Ax". Similar to App5 (shared-memory for both cores).
      - Conditions: caches L1 and L2 used.
Tools:
  - Vivado 2018.1
  - SDK
  - Zybo featuring Zynq-7010

Repository structure:
  - Vivado projects:
    - "PlatformHW_1_GPIO_wrapper.tcl": may be used to load and modify Vivado project (modify hardware and board configuration).
  - SDK applications: they run in ARM cores basing on PlatformHW_1_GPIO Vivado platform and PlatformHW_1_GPIO_bsp.
    - "App_1_GPIOAndHelloWorld".
    - "App_2_ARM_Private_Interrupt".
    - "App_3_Multicore_Simple_A0" (core0) and "App_3_Multicore_Simple_A1" (core1)
    - "App_4_Multicore_SharedInterr_A0" (core0) and "App_4_Multicore_SharedInterr_A1" (core1)
    - "App_5_Multicore_SharedMemory_A0" (core0) and "App_5_Multicore_SharedMemory_A1" (core1)
    - "App_6_Multicore_SharedMemory_L1_A0" (core0) and "App_6_Multicore_SharedMemory_L1_A1" (core1)
    - "App_7_Multicore_SharedMemory_L2_A0" (core0) and "App_7_Multicore_SharedMemory_L2_A1" (core1)
    - Apps 1-7 use "PlatformHW_1_GPIO_wrapper" for platform configuration / bitstream, "FSBL" as FSBL and "PlatformHW_1_GPIO_bsp" as bsp.
