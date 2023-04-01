<?php
    function Factorial_Function($input){
        if($input <= 1){
            return 1;
        }
        else{
            return $input * Factorial_Function($input - 1);
        }
    }
    $fact = Factorial_Function(6);
    echo "The factorial of the number 6 is $fact"
?>
