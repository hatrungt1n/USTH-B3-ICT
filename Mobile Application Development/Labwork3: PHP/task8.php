<?php
function connectDB() {
    $servername = "localhost";
    $username = "root";
    $password = "";
    $dbname = "students";
    $conn = new mysqli($servername, $username, $password, $dbname);
    
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    } else {
        echo "Connected successfully<br>";
    }
    return $conn;
}

function query($conn, $sql) {
    if ($conn->query($sql) === TRUE) {
        echo "Query successfully<br>";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}

$insert_data = "INSERT INTO student_infors (ID, NAME, CLASS, MARK, SEX)
VALUES 
(1, 'John Doe', 'Four', 75, 'female'), 
(2, 'Max Ruin', 'Three', 85, 'male'),
(3, 'Arnold', 'Three', 60, 'male'),
(4, 'Krish Star', 'Four', 60, 'female'),
(5, 'John Mike', 'Four', 60, 'female'),
(6, 'Alex John', 'Four', 55, 'male'),
(7, 'My John Rob', 'Fifth', 78, 'male'),
(8, 'Asruid', 'Five', 85, 'male'),
(9, 'Tes Qry', 'Six', 78, 'male'),
(10, 'Big John', 'Four', 55, 'female')
";

$delete_data = "DELETE FROM student_infors";
$update_mark = "UPDATE student_infors SET  CLASS='Two' WHERE MARK < 60";
$best_students = "SELECT * FROM student_infors WHERE MARK > 75";
$good_students = "SELECT * FROM student_infors WHERE MARK > 60 AND MARK <= 75";
$average_students = "SELECT * FROM student_infors WHERE MARK <= 60";

$conn = connectDB();
query($conn, $delete_data);
query($conn, $insert_data);
query($conn, $update_mark);


function show_students ($conn, $sql, $table_name) {
    echo "$table_name<br>";
    $result = $conn->query($sql);
    if (mysqli_num_rows($result) > 0) {
        echo "<table border='1'>";
        echo "<tr>";
        echo "<th>ID</th>";
        echo "<th>Name</th>";
        echo "<th>Class</th>";
        echo "<th>Mark</th>";
        echo "<th>Sex</th>";
        echo "</tr>";
        
        while($row = mysqli_fetch_assoc($result)) {
            echo "<tr>";
            echo "<td>" . $row["Id"] . "</td>";
            echo "<td>" . $row["Name"] . "</td>";
            echo "<td>" . $row["Class"] . "</td>";
            echo "<td>" . $row["Mark"] . "</td>";
            echo "<td>" . $row["Sex"] . "</td>";
            echo "</tr>";
        }
        echo "</table>";
    } else {
        echo "0 results";
    }
}

show_students($conn, $best_students, "Best students");
show_students($conn, $good_students, "Good students");
show_students($conn, $average_students, "Average students");
?>