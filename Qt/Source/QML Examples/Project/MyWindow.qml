import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: mainWindow
    width: 1200
    height: 800
    visible: true
    title: "QML menu bar"
    color: "#FFFFFF"

    menuBar: MenuBar {
		MenuBarItem {
			menu: Menu {
				title: "文件"
				Menu  {
					title: "新建"
					Menu
					{
						title: "语言"
						MenuItem {
							text: "Objective-C"
						}
						MenuItem {
							text: "C"
						}
						MenuItem {
							text: "C++"
						}
						MenuItem {
							text: "Rust"
						}
						MenuItem {
							text: "Java"
						}
						MenuItem {
							text: "C#"
						}
						MenuItem {
							text: "Go"
						}
						MenuItem {
							text: "Python"
						}
						MenuItem {
							text: "Ruby"
						}
						MenuItem {
							text: "Swift"
						}
						MenuItem {
							text: "Kotlin"
						}
						MenuItem {
							text: "PHP"
						}	
						MenuItem {
							text: "Visual Basic"
						}
						MenuItem {
							text: "Pascal"
						}
						MenuItem {
							text: "Ada"
						}	
						MenuItem {
							text: "Perl"
						}							
						MenuItem {
							text: "Assembly Language"
						}								
						MenuItem {
							text: "Html"
						}
						MenuItem {
							text: "CSS"
						}
						MenuItem {
							text: "Html"
						}
						MenuItem {
							text: "Java Script"
						}
					}
					Menu
					{
						title: "脚本"
						MenuItem {
							text: "CMakeLists.txt"
						}
						MenuItem {
							text: "Makefile"
						}
						MenuItem {
							text: "Meson"
						}
						MenuItem {
							text: "Ninja"
						}
						MenuItem {
							text: "Shell Scrpit"
						}
						MenuItem {
							text: "Go"
						}
					}
					Menu
					{
						title: "库"
						Menu {
							title: "OpenGL"
							
							MenuItem {
								text: "OpenGL 3.3"
							}
							MenuItem {
								text: "OpenGL 4.6"
							}
							MenuItem {
								text: "OpenGL ES 2.0"
							}
							MenuItem {
								text: "OpenGL ES 3.0"
							}
						}
						MenuItem {
							text: "Vulkan"
						}
						MenuItem {
							text: "D3D12"
						}
						MenuItem {
							text: "D3D11"
						}
						MenuItem {
							text: "D3D10"
						}
						MenuItem {
							text: "Metal"
						}
						MenuItem {
							text: "OpenCL"
						}
						MenuItem {
							text: "OpenAL"
						}
						MenuItem {
							text: "OpenCV"
						}
						MenuItem {
							text: "OpenVDB"
						}
					}
				}
				MenuItem {
					text: "打开"
				}
				MenuItem {
					text: "关闭项目"
				}
				MenuItem {
					text: "最近项目"
				}
				MenuItem {
					text: "退出"
				}			
			}
		}
		MenuBarItem {
			menu: Menu {
				title: "编辑"
				MenuItem {
					text: "撤销退格"
				}
				MenuItem {
					text: "重做"
				}
				MenuItem {
					text: "剪切"
				}
				MenuItem {
					text: "复制"
				}
			}
		}
		MenuBarItem {
			menu: Menu {
				title: "文档"
				Menu {
					title: "Arch"
					MenuItem {
						text: "ARM"
					}
					MenuItem {
						text: "ARM64"
					}
					MenuItem {
						text: "x86"
					}
					MenuItem {
						text: "x86-64"
					}
					MenuItem {
						text: "PowerPC"
					}
					MenuItem {
						text: "RISC-V"
					}
					MenuItem {
						text: "SPARC"
					}
					MenuItem {
						text: "Alpha"
					}
					MenuItem {
						text: "m68k"
					}
					MenuItem {
						text: "s390"
					}
					MenuItem {
						text: "Itanium"
					}	
					MenuItem {
						text: "MIPS"
					}	
					MenuItem {
						text: "LoongArch"
					}	
					MenuItem {
						text: "s390"
					}	
					MenuItem {
						text: "RISC-V"
					}						
				}
				Menu {
					title: "Bus"
					MenuItem {
						text: "USB"
					}
					MenuItem {
						text: "IIC"
					}
					MenuItem {
						text: "SPI"
					}
					MenuItem {
						text: "CAN"
					}
					MenuItem {
						text: "UART/USART"
					}
					MenuItem {
						text: "1-Wire"
					}
					MenuItem {
						text: "Parallel Bus"
					}
					MenuItem {
						text: "I2S"
					}
					MenuItem {
						text: "SDIO"
					}
					MenuItem {
						text: "PCIe"
					}
					MenuItem {
						text: "RS232"
					}	
					MenuItem {
						text: "RS485"
					}						
				}
				Menu {
					title: "无线通信技术"
					MenuItem {
						text: "Wifi"
					}
					MenuItem {
						text: "Bluetooth"
					}
					MenuItem {
						text: "NFC"
					}
					MenuItem {
						text: "Zigbee "
					}
					MenuItem {
						text: "5G"
					}					
				}
				Menu {
					title: "操作系统"
					MenuItem {
						text: "Linux"
					}
					MenuItem {
						text: "FreeRTOS"
					}
					MenuItem {
						text: "RThread"
					}
					MenuItem {
						text: "μC/OS-II"
					}	
					MenuItem {
						text: "μC/OS-III"
					}
					MenuItem {
						text: "μClinux"
					}
					MenuItem {
						text: "Vxworks"
					}
					MenuItem {
						text: "QNX"
					}
					MenuItem {
						text: "mbed OS"
					}
					MenuItem {
						text: "eCos"
					}		
					MenuItem {
						text: "RTX"
					}	
					MenuItem {
						text: "Alios Things"
					}	
					MenuItem {
						text: "Huawei LiteOS"
					}	
					MenuItem {
						text: "SylixOS"
					}	
					MenuItem {
						text: "djyos"
					}		
				}
				Menu
				{
					title: "语言"
					MenuItem {
						text: "Objective-C"
					}
					MenuItem {
						text: "C"
					}
					MenuItem {
						text: "C++"
					}
					MenuItem {
						text: "Rust"
					}
					MenuItem {
						text: "Java"
					}
					MenuItem {
						text: "C#"
					}
					MenuItem {
						text: "Go"
					}
					MenuItem {
						text: "Python"
					}
					MenuItem {
						text: "Ruby"
					}
					MenuItem {
						text: "Swift"
					}
					MenuItem {
						text: "Kotlin"
					}
					MenuItem {
						text: "PHP"
					}	
					MenuItem {
						text: "Visual Basic"
					}
					MenuItem {
						text: "Pascal"
					}
					MenuItem {
						text: "Ada"
					}	
					MenuItem {
						text: "Perl"
					}							
					MenuItem {
						text: "Assembly Language"
					}								
					MenuItem {
						text: "Html"
					}
					MenuItem {
						text: "CSS"
					}
					MenuItem {
						text: "Html"
					}
					MenuItem {
						text: "Java Script"
					}
				}
				Menu
				{
					title: "脚本"
					MenuItem {
						text: "CMakeLists.txt"
					}
					MenuItem {
						text: "Makefile"
					}
					MenuItem {
						text: "Meson"
					}
					MenuItem {
						text: "Ninja"
					}
					MenuItem {
						text: "Shell Scrpit"
					}
				}
				Menu
				{
					title: "库"
					Menu {
						title: "OpenGL"
						
						MenuItem {
							text: "OpenGL 3.3"
						}
						MenuItem {
							text: "OpenGL 4.6"
						}
						MenuItem {
							text: "OpenGL ES 2.0"
						}
						MenuItem {
							text: "OpenGL ES 3.0"
						}
					}
					MenuItem {
						text: "Vulkan"
					}
					MenuItem {
						text: "D3D12"
					}
					MenuItem {
						text: "D3D11"
					}
					MenuItem {
						text: "D3D10"
					}
					MenuItem {
						text: "Metal"
					}
					MenuItem {
						text: "OpenCL"
					}
					MenuItem {
						text: "OpenAL"
					}
					MenuItem {
						text: "OpenCV"
					}
					MenuItem {
						text: "OpenVDB"
					}
				}
				Menu
				{
					title: "数据结构"
					MenuItem {
						text: "链表"
					}
					MenuItem {
						text: "树"
					}
					MenuItem {
						text: "图"
					}
					MenuItem {
						text: "红黑树"
					}
					MenuItem {
						text: "AVL树"
					}
				}
				Menu
				{
					title: "数据库"
					Menu {
						title: "关系型数据库"
						MenuItem {
							text: "Mysql"
						}
						MenuItem {
							text: "Oracle"
						}
						MenuItem {
							text: "Sqlite"
						}
						MenuItem {
							text: "PostgreSQL"
						}
						MenuItem {
							text: "MariaDB"
						}
					}					
					Menu {
						title: "非关系型数据库"
						Menu {
							title: "文档数据库"
							MenuItem {
								text: "MongoDB"
							}
							MenuItem {
								text: "Couchbase"
							}
							MenuItem {
								text: "Firebase"
							}
						}
						Menu {
							title: "键值数据库"
							MenuItem {
								text: "Redis"
							}
							MenuItem {
								text: "Amazon DynamoDB"
							}
							MenuItem {
								text: "etcd"
							}
						}
						Menu {
							title: "列族数据库"
							MenuItem {
								text: "Apache Cassandra"
							}
							MenuItem {
								text: "HBase"
							}
							MenuItem {
								text: "ScyllaDB"
							}
						}
						Menu {
							title: "图数据库"
							MenuItem {
								text: "Neo4j"
							}
							MenuItem {
								text: "Amazon Neptune"
							}
							MenuItem {
								text: "JanusGraph"
							}

						}
						Menu {
							title: "时序数据库"
							MenuItem {
								text: "InfluxDB"
							}
							MenuItem {
								text: "Prometheus"
							}
							MenuItem {
								text: "TimescaleDB"
							}
						}							
					}
				}
				Menu
				{
					title: "网络协议"
					Menu {
						title: "应用层协议"
						
						MenuItem {
							text: "HTTP"
						}
						MenuItem {
							text: "FTP"
						}
						MenuItem {
							text: "TFTP"
						}
						MenuItem {
							text: "SMTP"
						}
						MenuItem {
							text: "POP3"
						}
						MenuItem {
							text: "IMAP"
						}
						MenuItem {
							text: "DNS"
						}
						MenuItem {
							text: "Telnet"
						}
						MenuItem {
							text: "SSH"
						}
						MenuItem {
							text: "SNMP"
						}
						MenuItem {
							text: "DHCP"
						}
						MenuItem {
							text: "SNMP"
						}					
					}
				}
			}
		}
	}
}