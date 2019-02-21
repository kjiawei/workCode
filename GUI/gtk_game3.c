#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <string.h>
#include <stdio.h>
//编译命令：gcc -g game3.c -o game3　`pkg-config --cflags --libs gtk+-2.0`
//编译最终版本时加上-O1或-O2优化，建议使用-Wall，显示详细的警告信息
//如果是windows版本可以加上-mwindows，这样从我的电脑里双击打开时就不会显示命令符窗口了，不过命令提示符中显示的一些程序的状态也就显示不了了

//static void destroy(GtkWidget*,gpointer);
//static void hello(GtkWidget *,gpointer);//gpointer表示连接事件时指定的一个指针(不一定是控件)
void toggle_statusbar(GtkWidget *, gpointer);

void undo_redo(GtkWidget *widget,  gpointer item) 
{
  static int count = 2;
  const char *name = gtk_widget_get_name(widget);

  if ( strcmp(name, "undo") ) {
    count++;
  } else {
    count--;
  }
 
  if (count < 0) {
     gtk_widget_set_sensitive(widget, FALSE);
     gtk_widget_set_sensitive(item, TRUE);
  } 

  if (count > 5) {
     gtk_widget_set_sensitive(widget, FALSE);//激活按钮
     gtk_widget_set_sensitive(item, TRUE);
  }
}

int main(int argc,char *argv[])
{
	//定义控件,按框架顺序排序
	GtkWidget *window;
	GtkWidget *vbox;//垂直布局容器控件
	GtkWidget *menubar;
	
	GtkWidget *fixed;//容器控件
	//GtkWidget *new;
	//GtkWidget *open;
	GtkWidget *button1;
	GtkWidget *button2;
	GtkWidget *button3;
	GtkWidget *filemenu;
	GtkWidget *file;
	GtkWidget *quit;
	GtkWidget *view;
	GtkWidget *viewmenu;
	GtkWidget *tog_stat;
	GtkWidget *statusbar;//带选择框的菜单
	
	GtkWidget *toolbar;
	//GtkWidget *exit;
	GtkToolItem *new;
	GtkToolItem *open;
	GtkToolItem *save;
	GtkToolItem *sep;
	GtkToolItem *exit;
	GtkToolItem *redo;
	GtkToolItem *undo;
	
	//GtkWidget *sep;//水平分割线控件?
	
	GtkAccelGroup *accel_group = NULL;//快捷键
	
	gtk_init(&argc,&argv);
	
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window),250,200);
	gtk_window_set_title(GTK_WINDOW(window),"menu");//三角形求面积,gtk只认utf8编码
	//gtk_container_set_border_width (GTK_CONTAINER (window), 25);
	//gtk_widget_set_size_request(window,200,100);
	
	fixed = gtk_fixed_new();
  gtk_container_add(GTK_CONTAINER(window), fixed);
	
	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	
	toolbar = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	gtk_container_set_border_width(GTK_CONTAINER(toolbar),2);
	
	undo = gtk_tool_button_new_from_stock(GTK_STOCK_UNDO);
	gtk_widget_set_name(GTK_WIDGET(undo),"undo");
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), undo, -1);
	
	redo = gtk_tool_button_new_from_stock(GTK_STOCK_REDO);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), redo, -1);
	
	new = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new, -1);
	
	open = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open, -1);

	save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, -1);

	sep = gtk_separator_tool_item_new();
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), sep, -1); 

	exit = gtk_tool_button_new_from_stock(GTK_STOCK_QUIT);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), exit, -1);
	
	gtk_box_pack_end(GTK_BOX(vbox),toolbar,FALSE,TRUE,5);
	
	//viewmenu与filemenu等有什么关系
	menubar = gtk_menu_bar_new();//生成菜单栏构件（menubar）
	viewmenu = gtk_menu_new();
	//filemenu = gtk_menu_new();//生成菜单构件(menu)
	
	view = gtk_menu_item_new_with_label("View");
	tog_stat = gtk_check_menu_item_new_with_label("View Statusbar");
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(tog_stat),TRUE);
	
	//accel_group = gtk_accel_group_new();
	//gtk_window_add_accel_group(GTK_WINDOW(window),accel_group);
	
	//没有图像
	/*file = gtk_menu_item_new_with_label("_File");
	new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, NULL);
	open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
	sep = gtk_separator_menu_item_new();
	quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT,accel_group);
	//quit = gtk_menu_item_new_with_label("Quit");
	gtk_widget_add_accelerator(quit,"activate",accel_group,GDK_q,GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);*/
	
	
	
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(view),viewmenu);
	gtk_menu_shell_append(GTK_MENU_SHELL(viewmenu), tog_stat);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar),view);
	gtk_box_pack_start(GTK_BOX(vbox),menubar,FALSE,FALSE,3);
	
	statusbar = gtk_statusbar_new();
	//gtk_box_pack_end(GTK_BOX(vbox),statusbar,FALSE,TRUE,1);
	
	//gtk_menu_item_set_submenu(GTK_MENU_ITEM(file),filemenu);//生成“文件”菜单
	//gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), new);//菜单选项填加进菜单外壳
	//gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);
	//gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), sep);
	//gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
	//gtk_menu_shell_append(GTK_MENU_SHELL(menubar),file);
	//gtk_box_pack_start(GTK_BOX(vbox),menubar,FALSE,FALSE,3);
	
	//状态，设置button的技巧
	//button = gtk_button_new_with_mnemonic("_Close");//文字是Close，C带下划线，用户单击Alt+C可以访问这个按钮，这是设计图形界面时很经常的做法
	//gtk_container_add(GTK_CONTAINER(window),button);
	button1 = gtk_button_new_with_label("Button");
  gtk_fixed_put(GTK_FIXED(fixed), button1, 150, 50);
  gtk_widget_set_size_request(button1, 80, 35);

  button2 = gtk_button_new_with_label("Button");
  gtk_fixed_put(GTK_FIXED(fixed), button2, 15, 15);
  gtk_widget_set_size_request(button2, 80, 35);

  button3 = gtk_button_new_with_label("Button");
  gtk_fixed_put(GTK_FIXED(fixed), button3, 100, 100);
  gtk_widget_set_size_request(button3, 80, 35);
	
	//信号区
	//g_signal_connect (G_OBJECT (window), "destroy",G_CALLBACK (destroy), NULL);//关闭窗体后退出
	//字符串表示的那个是信号，这个是gtk预先定义好的，在这里表示窗体被销毁信号，必须用这个名字，而后面的destroy要和我们定义的函数同名，如果函数名改了，这个也要相应修改
	//g_signal_connect_swapped (G_OBJECT (button), "clicked", G_CALLBACK (gtk_widget_destroy), (gpointer) window);//销毁窗体,窗体被销毁（destroy）是和gtk_main_quit()相连的，程序就会正常退出
	//g_signal_connect(G_OBJECT (button), "clicked", G_CALLBACK (hello), (gpointer) window);
	//g_signal_connect(G_OBJECT (quit), "activate", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT (undo), "clicked", G_CALLBACK (undo_redo), redo);
	g_signal_connect(G_OBJECT (exit), "clicked", G_CALLBACK (undo_redo), undo);
	
	g_signal_connect_swapped (G_OBJECT (window), "destroy", G_CALLBACK (gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT (exit), "clicked", G_CALLBACK (gtk_main_quit), NULL);
	//g_signal_connect(G_OBJECT (tog_stat), "activate", G_CALLBACK (toggle_statusbar), statusbar);
	
	gtk_widget_show_all(window);
	
	gtk_main();
	
	return 0;
}

/*static void destroy(GtkWidget *window,gpointer data)
{
gtk_main_quit ();
}

static void hello(GtkWidget *button,gpointer window)
{
gtk_window_set_title (GTK_WINDOW(window),"请保存文件");
}*/

void toggle_statusbar(GtkWidget *widget, gpointer statusbar) 
{
  if (gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(widget))) {
    gtk_widget_show(statusbar);
  } else {
    gtk_widget_hide(statusbar);
  }
}
