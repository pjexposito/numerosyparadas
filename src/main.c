#include <pebble.h>

Window* window;
TextLayer *text_layer, *text_layer2, *t1_layer, *t2_layer;
int numero=0, letra=0, posicion=0;
char texto[1024];

static char buf[] = "123456";
const char *lineas[]= {"A","B","C","D","E","F","G","H"};
int tamano = sizeof(lineas)/sizeof(lineas[0]);


char t1_buffer[64], t2_buffer[64];

enum {
	KEY_T1 = 0,
	KEY_T2 = 1
};



void process_tuple(Tuple *t)
{
	//Get key
	int key = t->key;

	//Get integer value, if present
	
  //int value = t->value->int32;
	char tiempo1[64];
	strcpy(tiempo1, t->value->cstring);
  

  char tiempo2[64];
	strcpy(tiempo2, t->value->cstring);

	//Decide what to do
	switch(key) {
		case KEY_T1:
			//Location received
			snprintf(t1_buffer, sizeof("XXXXXX"), "%s", tiempo1);
			text_layer_set_text(t1_layer, (char*) &t1_buffer);
			break;
		case KEY_T2:
			//Temperature received
			//snprintf(temperature_buffer, sizeof("Temperature: XX \u00B0C"), "Temperature: %d \u00B0C", value);
      snprintf(t2_buffer, sizeof("XXXXXX"), "%s", tiempo2);
			text_layer_set_text(t2_layer, (char*) &t2_buffer);
			break;
	}

}

static void in_received_handler(DictionaryIterator *iter, void *context) 
{
	(void) context;
	
	//Get data
	Tuple *t = dict_read_first(iter);
	if(t)
	{
		process_tuple(t);
	}
	
	//Get next
	while(t != NULL)
	{
		t = dict_read_next(iter);
		if(t)
		{
			process_tuple(t);
		}
	}
}

void send_int(int8_t parada, const char *linea)
{
	DictionaryIterator *iter;
 	app_message_outbox_begin(&iter);
 	/*
 	Tuplet value = TupletInteger(KEY_T1, parada);
  Tuplet value2 = TupletInteger(KEY_T2, linea);
 	dict_write_tuplet(iter, &value);
 	dict_write_tuplet(iter, &value2); 	
  */
  dict_write_int8(iter, KEY_T1, parada);
  dict_write_cstring(iter, KEY_T2, linea);
  dict_write_end(iter);

  
 	app_message_outbox_send();
  
}

void up_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  
/*  
  
  //Esto que va a continuación se debe usar para añadir el valor de un int a una cadena de texto
  //Texto ahora es Pulsado: 
  strcpy(texto,"Pulsado: ");
  numero++;
  // Se convierte la variable número de decimal a char. Se guarda en buf
  snprintf(buf, sizeof(buf), "%d", numero);
  // Ahora, se añade el valor de buf (el número) a la cadena texto
  strcat(texto, buf);
  // Ahora que texto tiene todo el valor, se muestra.
  text_layer_set_text(text_layer, texto);
  
*/
  if (posicion==0){
    if (numero==9)
      numero=0;
    else
      numero++;
    snprintf(buf, sizeof(buf), "%d", numero);
	  text_layer_set_text(text_layer, buf);
  }
  else
  {
   letra++;
   if (letra==tamano)
    letra=0;
   text_layer_set_text(text_layer2, lineas[letra]); 
  }  

}

void down_click_handler(ClickRecognizerRef recognizer, void *context) 
{
  if (posicion==0){
    if (numero==0) 
      numero=9;
    else
      numero--;
    snprintf(buf, sizeof(buf), "%d", numero);
	  text_layer_set_text(text_layer, buf);
  }
  else
  {
   if (letra==0)
     letra=tamano;
   letra--;

   text_layer_set_text(text_layer2, lineas[letra]); 
  }  
}

void select_click_handler(ClickRecognizerRef recognizer, void *context)
{
	//text_layer_set_text(text_layer, "You pressed SELECT!");

  if (posicion==1)
    {
    text_layer_set_text(text_layer, "X");
    
    send_int(numero,lineas[letra]);
    }
  if (posicion==0) 
    posicion=1;


}





void click_config_provider(void *context) 
{
	window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
	window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}

/* Load all Window sub-elements */
void window_load(Window *window)
{
  t1_layer = text_layer_create(GRect(0, 0, 20, 20));
	text_layer_set_background_color(t1_layer, GColorBlack);
	text_layer_set_text_color(t1_layer, GColorWhite);
	layer_add_child(window_get_root_layer(window), (Layer*) t1_layer);
	text_layer_set_text(t1_layer, "0");
  
  
  t2_layer = text_layer_create(GRect(20, 0, 20, 20));
	text_layer_set_background_color(t2_layer, GColorBlack);
	text_layer_set_text_color(t2_layer, GColorWhite);
	layer_add_child(window_get_root_layer(window), (Layer*) t2_layer);
	text_layer_set_text(t2_layer, "0");  
  
  
	text_layer = text_layer_create(GRect(20, 40, 40, 60));
  text_layer_set_font(text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_background_color(text_layer, GColorBlack);
	text_layer_set_text_color(text_layer, GColorWhite);
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), (Layer*) text_layer);
	text_layer_set_text(text_layer, "0");
  
  
  text_layer2 = text_layer_create(GRect(70, 40, 40, 60));
  text_layer_set_font(text_layer2, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
	text_layer_set_background_color(text_layer2, GColorBlack);
	text_layer_set_text_color(text_layer2, GColorWhite);
	text_layer_set_text_alignment(text_layer2, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), (Layer*) text_layer2);
	text_layer_set_text(text_layer2, "A");
  
  
}

/* Un-load all Window sub-elements */
void window_unload(Window *window)
{
	text_layer_destroy(text_layer);
  text_layer_destroy(text_layer2);
  text_layer_destroy(t1_layer);
  text_layer_destroy(t2_layer);
}

/* Initialize the main app elements */
void init()
{
	window = window_create();
	WindowHandlers handlers = {
		.load = window_load,
		.unload = window_unload
	};
  
	app_message_register_inbox_received(in_received_handler);					 
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());		//Largest possible input and output buffer sizes
	
	window_set_window_handlers(window, (WindowHandlers) handlers);
	window_set_click_config_provider(window, click_config_provider);
	window_stack_push(window, true);
}

/* De-initialize the main app elements */
void deinit()
{
	window_destroy(window);
}

/* Main app lifecycle */
int main(void)
{
	init();
	app_event_loop();
	deinit();
}