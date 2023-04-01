<?php
$songs = array("3. Love yourself", "1. Boyfriend", "2. Let me love you", "4. What do you mean", "5. Peaches");
sort($songs);
$songLength = count($songs);
for($x = 0; $x < $songLength; $x++) {
  echo $songs[$x];
  echo "<br>";
}
?>