
var dict;


function HTTPGET(url) {
	var req = new XMLHttpRequest();
  req.ontimeout = function() {console.log("The request timed out.");   }; 
  
  req.open("GET", url, false);

	req.send(null);

	return req.responseText;
}

function BuscaParadas(parada,linea) {

    console.log("http://www.auesa.es/paradas_qr/"+parada+".php?vari="+linea);

  // Usa Google para obtener la información
  var response = HTTPGET("http://www.auesa.es/paradas_qr/"+parada+".php?vari="+linea);
  
	//Convierte la respuesta de la web de Google en JSON, para parsearla
	var json = JSON.parse(response);
	

  
  var t1 = json.GetPasoParadaResult.PasoParada.e1.minutos;
  var t2 = json.GetPasoParadaResult.PasoParada.e2.minutos;

	
	//Mando a la consola los datos que he obtenido
  console.log("Tiempo:" + t1 + ", " + t2);
	
	//Este es el diccionario con los datos que voy a mandar al reloj
  dict = {"KEY_T1" : t1, "KEY_T2": t2};

	//Mando los datos de dirección al reloj
	Pebble.sendAppMessage(dict);
}



Pebble.addEventListener("ready",
  function(e) {
	//Se activa el reloj por primera vez. Se piden los datos por primera vez
  console.log("Evento Ready");

  }
);



Pebble.addEventListener("appmessage",
  function(e) {
    //Se refresca la información
    console.log("Received message: " + e.payload.KEY_T1 + " " + e.payload.KEY_T2);

  var parada=e.payload.KEY_T1;
  var linea=e.payload.KEY_T2;
  BuscaParadas(parada,linea);
  }
);