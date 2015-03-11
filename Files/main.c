/* Tea Package Maker's source code
 * March, 2015
 */


#include <gtk/gtk.h>
#include <stdbool.h>
#include <string.h>

	// define widgets
	GtkBuilder *builder;
	GtkWidget *main_window, *confirm_window,*warn,*invalid_window,*warn3,*success_window1,*info_window;
	GtkWidget *create_btn,*no_btn,*yes_btn,*open_btn,*warn_oka,*warn_okb,*invalid_btn,*success_ok1,*info_btn,*close_btn;
	GtkWidget *label_success1;
	GtkWidget *name;
	GtkWidget *list_dependency,*buff;

	// define additional variables
	gint a;
	gchar path[100];
	gchar path2[100];
	gint i,j;
	gchar command[999];


// exit the apps when main window is destroyed
void on_exit()
{
	system("rm -rf /tmp/TEA/"); 				// clean up temporary files
	gtk_main_quit ();							// exit the apps
}


//showing and hiding window
void hide_confirm()
{
	gtk_widget_hide(confirm_window);
}

void hide_warn()
{
	gtk_widget_hide(warn);
}

void hide_warn3()
{
	gtk_widget_hide(warn3);
}

void hide_invalid()
{
	gtk_widget_hide(invalid_window);
}
void hide_success()
{
	gtk_widget_hide(success_window1);
}
void hide_info()
{
	gtk_widget_hide(info_window);
}
void show_info()
{
	gtk_widget_show(info_window);
}

// print information to text view at confirmation window
void print_to_field()
{
	gchar *filestream=NULL;
	g_file_get_contents("/tmp/TEA/show",&filestream,NULL,NULL);
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(buff),filestream,-1);
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(list_dependency),GTK_TEXT_BUFFER(buff));
}

// print information if the operation was successful
void print_to_success()
{
	gchar *stream=NULL;
	snprintf(stream, sizeof(stream),"File %s_dan_dependensinya.tea\nberhasil disimpan di folder Home",gtk_entry_get_text (GTK_ENTRY(name)));
	gtk_text_buffer_set_text (GTK_TEXT_BUFFER(buff),stream,-1);
	gtk_text_view_set_buffer(GTK_TEXT_VIEW(list_dependency),GTK_TEXT_BUFFER(buff));
}

// get 'profile' path and convert it from 'file:///....' to '/../..'
void get_profile_path()
{
	snprintf(path, sizeof(path),"%s", gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(open_btn)));
	a=strlen(path);
	for(i=7,j=0;;)
	{
		if(j<=a)
		{
			path2[j]=path[i];
		
		}
		else
		{
			break;
		}
		i++,j++;
	}
}

// check software availability on the repository
bool check_available()
{
	gchar *tea_dep_c=NULL;
	
	snprintf(command, sizeof(command),
		"apt-get --print-uris -y -o dir::state::status=\"%s\" -o dir::cache::archives=\"$HOME/.Tea-package-maker\" install %s | grep '\\.deb' | wc -l >> /tmp/TEA/count",
		path2,gtk_entry_get_text (GTK_ENTRY(name)));
	system(command);
	g_file_get_contents ("/tmp/TEA/count",&tea_dep_c,NULL,NULL);	
	int x,x0;
	x = atoi(tea_dep_c);
	if(x == 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
	
}


// check software dependencies 
void check_depend()
{
	system("rm -rf /tmp/TEA ; mkdir /tmp/TEA/ ; mkdir /tmp/TEA/workspace/ ; mkdir /tmp/TEA/workspace/partial/");
	
	if(g_strcmp0(gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(open_btn)),NULL)==0) // check if user hasn't input the profile file
	{
		 gtk_widget_show (warn); // show warning
	}
	else
	{
		get_profile_path();
		if(check_available()==0)
		{
			gtk_widget_show (invalid_window); // show warning if the software is not available on the repository
		}
		else
		{
			gtk_widget_show(confirm_window); // show confirmation window
			system("rm /tmp/TEA/show && rm /tmp/TEA/list && > /tmp/TEA/list");
			snprintf(command, sizeof(command),
					 "export nama_aplikasi=\"%s\" konfirmasi=\"1\" profil=\"%s\"; /usr/share/TEA/engine/teamaker1",
					 gtk_entry_get_text (GTK_ENTRY(name)),path2);
			system(command);	
		}
		
	}
	print_to_field (); // print info
}

// continue progress when user click 'Ya' on confirmation window
void lanjutkan()
{
	gchar *tea_errchk,aa[999];
	gtk_widget_hide (confirm_window);
	snprintf(command, sizeof(command),
		 "export nama_aplikasi=\"%s\" konfirmasi=\"1\" profil=\"%s\"; /usr/share/TEA/engine/teamaker2",
		 gtk_entry_get_text (GTK_ENTRY(name)),path2);
	system(command);
	g_file_get_contents ("/tmp/TEA/errchk",&tea_errchk,NULL,NULL);
	int x11;
	x11 = atoi(tea_errchk);	
	if(x11==1)
	{
	gtk_widget_show (warn3); 
	}
	else
	{
		snprintf(aa, sizeof(aa), "File %s_dan_dependensinya.tea berhasil dibuat\nFile disimpan di folder Home",gtk_entry_get_text (GTK_ENTRY(name)));
		gtk_label_set_label (GTK_LABEL(label_success1),aa);
		gtk_widget_show (success_window1);
	}
		
}

// abort progress when user click 'tidak'
void batal()
{
	hide_confirm ();
}

int
main (int argc, char *argv[])
{
	gtk_init(&argc, &argv);
	
	// load ui file
	builder=gtk_builder_new();
	gtk_builder_add_from_file(builder, "/usr/share/TEA/UI/tea_package_maker.ui", NULL);

	//Get all object
	main_window			= GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
	confirm_window		= GTK_WIDGET(gtk_builder_get_object(builder, "confirmation1"));
	create_btn			= GTK_WIDGET(gtk_builder_get_object(builder, "create_btn1"));
	no_btn				= GTK_WIDGET(gtk_builder_get_object(builder, "no_btn1"));
	yes_btn				= GTK_WIDGET(gtk_builder_get_object(builder, "yes_btn1"));
	name				= GTK_WIDGET(gtk_builder_get_object(builder, "software_name1"));
	list_dependency		= GTK_WIDGET(gtk_builder_get_object(builder, "dependency1"));
	open_btn			= GTK_WIDGET(gtk_builder_get_object(builder, "open_btn1"));
	warn				= GTK_WIDGET(gtk_builder_get_object(builder,"warn_window1"));
	invalid_window		= GTK_WIDGET(gtk_builder_get_object(builder,"invalid_window1"));
	invalid_btn			= GTK_WIDGET(gtk_builder_get_object (builder, "invalid_btn1"));
	buff				= GTK_WIDGET(gtk_builder_get_object (builder,"buff1"));
	warn3				= GTK_WIDGET(gtk_builder_get_object(builder,"warn_window3"));
	warn_oka			= GTK_WIDGET(gtk_builder_get_object(builder,"warn_ok1"));
	warn_okb			= GTK_WIDGET(gtk_builder_get_object(builder,"warn_ok2"));
	success_window1		= GTK_WIDGET(gtk_builder_get_object(builder,"success_window"));
	label_success1		= GTK_WIDGET(gtk_builder_get_object(builder,"label_success"));
	success_ok1			= GTK_WIDGET(gtk_builder_get_object(builder,"success_ok"));
	info_btn			= GTK_WIDGET(gtk_builder_get_object (builder, "info_btn1"));
	info_window			= GTK_WIDGET(gtk_builder_get_object (builder, "info_window1"));
	close_btn			= GTK_WIDGET(gtk_builder_get_object (builder, "close_btn1"));
	gtk_builder_connect_signals(builder,NULL);

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(open_btn),"/usr/share/TEA/profiles/");

	// signals
	g_signal_connect(G_OBJECT(create_btn), "clicked", G_CALLBACK(check_depend), NULL);
	g_signal_connect(G_OBJECT(yes_btn), "clicked", G_CALLBACK(lanjutkan), NULL);
	g_signal_connect(G_OBJECT(no_btn), "clicked", G_CALLBACK(batal), NULL);
	g_signal_connect_swapped(main_window, "destroy", G_CALLBACK(on_exit), NULL);
	g_signal_connect(G_OBJECT(confirm_window),"delete_event",G_CALLBACK(hide_confirm),NULL);
	g_signal_connect(G_OBJECT(warn),"delete_event",G_CALLBACK(hide_warn),NULL);
	g_signal_connect(G_OBJECT(invalid_window),"delete_event",G_CALLBACK(hide_invalid),NULL);
	g_signal_connect(G_OBJECT(invalid_btn), "clicked", G_CALLBACK(hide_invalid), NULL);
	g_signal_connect(G_OBJECT(warn3),"delete_event",G_CALLBACK(hide_warn3),NULL);
	g_signal_connect(G_OBJECT(warn_oka), "clicked", G_CALLBACK(hide_warn), NULL);
	g_signal_connect(G_OBJECT(warn_okb), "clicked", G_CALLBACK(hide_warn3), NULL);
	g_signal_connect(G_OBJECT(success_ok1), "clicked", G_CALLBACK(hide_success), NULL);
	g_signal_connect(G_OBJECT(info_btn), "clicked", G_CALLBACK(show_info), NULL);
	g_signal_connect(G_OBJECT(close_btn), "clicked", G_CALLBACK(hide_info), NULL);
	g_signal_connect(G_OBJECT(info_window),"delete_event",G_CALLBACK(hide_info),NULL);
	g_object_unref(G_OBJECT(builder));
	gtk_widget_show(main_window);
	
	gtk_main();
	return 0;
}

