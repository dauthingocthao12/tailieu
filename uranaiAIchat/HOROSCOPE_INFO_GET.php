<?php
echo "aaaa";
define("HOROSCOPE_INFO_GET", "horoscope_info");

function db_connect() {
    $servername = "localhost";
    $username = "xs383367_uranaib";
    $password = "E2bUfth3";
    $dbname = "xs383367_uranailinebot";

    // Create connection
    try {
        $conn = new mysqli($servername, $username, $password, $dbname);

        // Check connection
        if (!$conn-> connect_errno) {
            // return $conn;
            return null; //todo
        } else {
            return false;
        }
    } catch (Exception $e) {
        get_horoscope_infor();
        return false;  
    }
}
function select($sql) {
    $data = [];
    if($db_connect = db_connect()) {
        $data = [];
        $result = $db_connect->query($sql);
        if ($result->num_rows > 0) {
            while($row = $result->fetch_assoc()) {
                $data[] = $row;
            }
        }
        $db_connect->close();
    }

    return $data;

}
function get_horoscope_infor(){
    $today = date('Y-m-d');
    $sql = "SELECT * FROM ".HOROSCOPE_INFO_GET." WHERE get_date= $today";
    $data = select($sql);
}
?>