<?php

// DB設定ファイルの読み込み
require_once("../../uranai_lib/libadmin/config.php");

// 返却する配列
$result = [
    'sql_result' => null,
    'error' => null
];

// エラーメッセージ
$ERROE_MSG_USER_PASSWORD = "ユーザー名、又はパスワードが間違っています";
$ERROE_MSG_ACTION = "送信内容が不正です";
$ERROE_MSG_CONNECTION = "データベースに接続できませんでした";
$ERROE_MSG_SQL = "SQLのエラー";

// POSTされてきたjsonデータを受け取る
$json = file_get_contents('php://input');

// JSON形式から配列へ変換する
$data = json_decode($json, true);

// エラーハンドリング
if($data['user'] != "uranairanking") {
    $result['error'] = $ERROE_MSG_USER_PASSWORD;
    echo json_encode($result);
    return;
}

if($data['password'] != "H2yu4TXM") {
    $result['error'] = $ERROE_MSG_USER_PASSWORD;
    echo json_encode($result);
    return;
}

if($data['action'] != "SELECT") {
    $result['error'] =  $ERROE_MSG_ACTION;
    echo json_encode($result);
    return;
}

// DB接続
$conn = mysqli_connect(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
if (!$conn) {
    $result['error'] =  $ERROE_MSG_CONNECTION;
    echo json_encode($result);
    return;
}

// 今日の日付
$today = date("Y-m-d");

$sql = "SELECT COUNT(*) AS cnt FROM `log` WHERE `is_delete` = 0 AND `day` = '".$today."';";
if (!($rs = mysqli_query($conn, $sql))){
    $result['error'] =  $ERROE_MSG_SQL;
    echo json_encode($result);
    return;
}

$row = $rs->fetch_assoc();

$result['sql_result'] = $row['cnt'];

echo json_encode($result, JSON_UNESCAPED_UNICODE);
