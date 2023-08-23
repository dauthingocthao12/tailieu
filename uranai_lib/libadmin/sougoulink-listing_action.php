<?php 

require_once __DIR__ . '/vendor/autoload.php';
require_once dirname(__FILE__)."/../../uranai_lib/libs/Smarty.class.php" ;


function confirmed($id, $url, $site_name_kana){
    global $conn;

    $site_info = site_info($id);
    $us_link = "https://uranairanking.jp/link_list/index.php?share=" . $id;
    $insert_values = "'" .$site_info["site_name"] . "','" . $site_name_kana . "','" . $url . "','" .  $us_link . "','" . $site_info["email"] . "'";
    
    $sql = "INSERT INTO sougo_sites (site_name, site_name_kana, their_link ,us_link, email) VALUES($insert_values)";
    if (mysqli_query($conn, $sql)) {
        $last_id = mysqli_insert_id($conn);
        $sql = "UPDATE sougo_sites_nominated SET confirmed = 1 , management_number = $last_id where id = $id";
        mysqli_query($conn, $sql);
    }
    
}

function site_info($id){
    global $conn;

    $sql = "SELECT * FROM sougo_sites_nominated where id = $id";
    $result = mysqli_query($conn, $sql);

    if (mysqli_num_rows($result) > 0) {
        while($row = mysqli_fetch_assoc($result)) {
            return ["site_name" => $row["site_name"], "url" => $row["url"], "email" => $row["email"], "confirmed" => $row["confirmed"], "management_number" => $row["management_number"]];
        }
    }else {

    }
    
}

function deleted($id){
    global $conn;
  
    $site_info = site_info($id);
    if($site_info["confirmed"] == 1){
        $sougo_sites_id = $site_info["management_number"];
        $sql = "UPDATE sougo_sites SET is_delete = 1 where id = $sougo_sites_id";
        mysqli_query($conn, $sql);
        
    }
    $sql = "UPDATE sougo_sites_nominated SET is_denied = 1 WHERE id = $id";
    mysqli_query($conn, $sql);
 
}

function getNominatedSites(){
    global $conn;
    $sql = "SELECT * FROM sougo_sites_nominated";
    $result = mysqli_query($conn, $sql);
    $sites_list = null;
    if (mysqli_num_rows($result) > 0) {
        while($row = mysqli_fetch_assoc($result)) {
            if(!$row["is_denied"]){
                $sites_list[$row["id"]] = ["site_name" => $row["site_name"], "url" => $row["url"], "email" => $row["email"], 
                "confirmed" => $row["confirmed"], "management_number" => $row["management_number"],"is_denied" => $row["is_denied"] ];
            }
            
        }
    }
    return $sites_list;
}

function getConfirmedSites(){
    global $conn;
    $sql = "SELECT * FROM sougo_sites WHERE is_delete = 0";
    $sql .= " ORDER BY site_name_kana;";
    $result = mysqli_query($conn, $sql);
    $sites_list = null;
    if (mysqli_num_rows($result) > 0) {
        while($row = mysqli_fetch_assoc($result)) {
            $sites_list[$row["id"]] = [
                "site_name" => $row["site_name"], 
                "their_link" => $row["their_link"], 
                "us_link" => $row["us_link"], 
                "email" => $row["email"],
                "is_delete" => $row["is_delete"],
                "site_name_kana" => $row["site_name_kana"]
            ];
        }
    }
    return $sites_list;
}

function update($id, $name, $name_kana, $url, $mail){
    global $conn;
    $sql = "UPDATE sougo_sites_nominated SET site_name = '$name' WHERE management_number = $id";
    mysqli_query($conn, $sql);
    $sql = "UPDATE sougo_sites SET site_name = '$name', site_name_kana = '$name_kana', their_link = '$url', email = '$mail' WHERE id = $id";
    mysqli_query($conn, $sql);
    
}

function getConfirmedSitesWithKey(){
    global $conn;
    $sql = "SELECT ssn.id, ss.site_name, ss.email, ss.their_link";
    $sql .= " FROM sougo_sites as ss";
    $sql .= " JOIN sougo_sites_nominated as ssn ON ss.id = ssn.management_number";
    $sql .= " AND ss.is_delete = 0";
    $list = [];
    $result = mysqli_query($conn, $sql);
    if (mysqli_num_rows($result) > 0) {
        while($row = mysqli_fetch_assoc($result)) {
            $list[$row["id"]] = ["site_name" => $row["site_name"], "email" => $row["email"], "their_link" => $row["their_link"]];
         }
    }
    return $list;
    
}
?>