arm-pkg-packer说明：

该工具用来打包arm更新文件，最终生成.pkg的更新文件。

使用方式：

1. 使用pack.cfg配置文件来配置打包的内容，示例如下：

tag=KERNEL input=kernel.bin
tag=ROOTFS input=rootfs.bin
tag=APPFS input=appfs.bin

可视需要灵活配置；并把相应指定的bin文件放置到当前目录；

2. 运行packer命令行工具自动打包。使用方式如下：

usage: ./packer -P PRODUCT_MODEL -C cfg_filename -o pkg_filename 
other arguments: -v(-version), -h(-?, -help)


