# NoirVisor

NoirVisor - Hardware-Accelerated Hypervisor solution with support to complex functions and purposes and nested virtualization.

# Introduction
NoirVisor is a hardware-accelerated hypervisor (a.k.a VMM, Virtual Machine Monitor) with support to complex functions and purposes. It is designed to support processors based on x86 architecture with hardware-accelerated virtualization feature. For example, Intel processors supporting Intel VT-x or AMD processors supporting AMD-V meet the requirement. By designation, NoirVisor determines the processor manufacturer and selects the function core.

# Processor Requirement
Intel Processors based on Intel 64 and IA-32 Architecture, with support to Intel VT-x. Intel EPT is prefered, but not required.
AMD Processors based on AMD64 Architecture, with support to AMD-V. Nested Paging is prefered, but not required.
Other processors based on x86 architecture may be supported in future.

# Nested Virtualization
NoirVisor is developed in highest focus on nested virtualization. It is not currently supported, but will be developed in future.

# Announcement to all contributors
NoirVisor is coded in the C programming language and the assembly since it is procedure-oriented designed.
DO NOT PROVIDE CODES WITH C++ WHICH INVOLVES THE NoirVisor Core IN YOUR PULL-REQUEST!

# Build
To build a kernel-mode driver on Windows, you should install Windows Driver Kit 7.1.0 to default path on C disk. Then run the provided batch file to build it.
Also note that, you have to create certain directories required by the batch complilation.
You may download the WDK 7.1.0 from Microsoft: https://www.microsoft.com/en-us/download/details.aspx?id=11800
Note that you should execute the build_prep.bat to make directories for first-time compilation.

# Supported Platforms
NoirVisor is designed to be cross-platform. It can be built to a kernel-mode component of an operating system, or even as a software with bootstrap running on bare-metal.
Currently, NoirVisor supports the Windows Operating System newer than or same as Windows XP, running as a kernel-mode driver.
If there is already a hypervisor running in the system, make sure it supports native virtualization nesting.

# Development Status
Project NoirVisor has finished developping the basic hypervisor on Intel VT-x.
Now NoirVisor has two future development plans:
Develop an AMD-V based hypervisor.
Develop a length-disassemble engine.

# Completed Features
Stealth SSDT Hook (NtOpenProcess Hook) on 64-bit Windows
Stealth Inline Hook (NtSetInformationFile Hook) on 64-bit Windows

# License
All the codes, except the BeaEngine, with NoirVisor are licensed under MIT License.
BeaEngine, as declared by BeatriX, is licensed under Lesser GPL v3. Therefore, partial code, under this repository, from BeaEngine are licensed under LGPL v3.