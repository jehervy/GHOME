<?php header('Content-Type: text/html; charset=utf-8') ?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">

    <head>        
        <title>Serveur Web : Rémi</title>
        <link rel="stylesheet" type="text/css" href="style.css" />
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    </head></html>
    <body>
       
      <?php
	$fp = stream_socket_client("127.0.0.1:2300", $errno, $errstr, 30);
	
	if (!$fp) 
	{
    		echo "Connexion echouee : $errstr ($errno)<br />\n";
	} 
	
	else 
	{
    		echo "Connexion reussie <br />";
           	 /* Send instructions. */
           	 //fwrite($fp, "YOO");
   		 //echo fread($fp, 26);
	}
	
	if (isset($_POST['submit']) AND isset($_POST['thermique']))
	{
		echo "Test de type 1 <br />";
		var_dump(fwrite($fp, $_POST['thermique']));
		
   		 echo fread($fp, 26);
   		 
	}
	
	if (isset($_POST['submit']) AND isset($_POST['presence']))
	{
		echo "Test de type 2 <br />";
		$string = $_POST['presence'];
		echo $string."<br />";
		var_dump(fwrite($fp, $string));
		echo fread($fp, 26);
	}
	fclose($fp);
	?>
	
	<form method="post" action="info.php"> 
		<p>
		    Transférer un message thermique :
		</p> 
		
		<input type="text" name="thermique" />
		
		<input type="submit" name="submit" value="test" />
	</form>
	
	<form method="post" action="info.php">
		<p>
			Transférer un message de présence :
		</p>
		<input type="text" name="presence" />
		<input type="submit" name="submit" value="test" />
	</form>
	
        
    </body>
</html>
