# Zynq-7000-Multicore-Baremetal

The main goal of this project is to analyse the Zynq-7000 architecture (with a Zybo board) in order to exploit the dual-core (ARM-Cortex A9) architecture in baremetal.

The project is structured in several applications that add features of the architecture step by step, starting from scratch. The flow is intended to go as follows:
  1) Basic test of PS (Processing System) and PL (Programmable Logic): using one ARM core, print helloworld and control a GPIO core implemented in PL to manipulate on-board LED state using Xilinx drivers for GPIO.
  2) Use of ARM-core-private interruptions (single core).
  3) Use both ARM cores to do a first approach of multicore (each core running some application similar to the previous one).
      - Conditions: both cores running independetly, each one running its own code and using its own private memory section.
  4) Use shared / intercore interruptions to communicate ARM both cores.
      - Conditions: both cores running independetly, each one running its own code and using its own private memory section.
  5) Add shared-memory for both cores. This will require them to communicate each other (using interruptions) and to use barriers to guarantee that instructions in pipeline accessing to shared-memory have finished before of notify the other core.
  6) ...
  
Tools:
  - Vivado 2018.1
  - SDK
  - Zybo featuring Zynq-7010

