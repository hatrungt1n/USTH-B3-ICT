<?php
function IsPrime($n)
{
 for($x=2; $x<$n; $x++)
   {
      if($n % $x ==0)
	      {
		   return 0;
		  }
    }
  return 1;
   }

$numToCheck = 17;
$a = IsPrime($numToCheck);
if ($a==0)
echo "$numToCheck is not a Prime Number"."\n";
else
echo "$numToCheck is a Prime Number"."\n";
?>
