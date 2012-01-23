<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">
    <head>        
        <title>Serveur Web : Rémi</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    </head>
    <body>
      <?php
		$fd = pfsockopen("127.0.0.1",3011, $errno, $errstr,30);
	if (isset($_POST['submit']) AND isset($_POST['apero']))
	{
		echo "CA RENTRE LA <br />";
		echo "File descriptor: ".$fd;

		//fwrite($fd, 2|8<<32|32<<64);
		//fwrite($fd, 2, 4);
		//fwrite($fd, 8, 4);
		//fwrite($fd, 32, 4);
		fwrite($fd, 13213251);
		   		 echo fread($fd, 26);
   		 
	}

	?>
	
	<form method="post" action="info.php"> 
		<p>
		    On insèrera ici les éléments de notre formulaire.
		</p> 
		
		<input type="text" name="apero" />
		
		<input type="submit" name="submit" value="test" />
	</form>
        
    </body>
</html>
