SECTIONS
{
	. = 0x40000000;
	.text : {
		start.o
		* (.text)
	}
	.data : {
		* (.data)
	}
	.bss : {
		* (.bss)
	}	
}
