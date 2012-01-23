<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="fr" lang="fr">
    <head>        
        <title>Serveur Web : Rémi</title>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    </head>
    <body>
      <?php
	$fd = pfsockopen("127.0.0.1",3006, $errno, $errstr,30);
	echo "File descriptor : ".$fd."<br />";
	
	if (!$fd) 
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
	
	if (isset($_POST['submit']) AND isset($_POST['apero']))
	{
		echo "CA RENTRE LA <br />";
		fwrite($fd, $_POST['apero']);
		
   		 echo fread($fd , 26);
   		 
	}
		?>
	
	<form method="post" action="message.php"> 
		<p>
		    On insèrera ici les éléments de notre formulaire.
		</p> 
		
		<input type="text" name="apero" />
		
		<input type="submit" name="submit" value="test" />
	</form>
        
    </body>
</html>

<?php session_write_close() ?>