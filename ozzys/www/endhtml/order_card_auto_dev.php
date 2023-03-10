<?php

// ================
// ！！！注意！！！
// このファイルは本番でテストするためのファイルです。
// 開発のDBのcone.incファイルを利用する。
// ================

//	基本ファイル読込
include("../../develop/cone.inc");
include("../../develop/www/sub/setup.inc");
include("../../develop/www/sub/array.inc");
include("../../develop/www/sub/webcollect.class.inc");


if($_POST['order_no']) {
    // webcollectから自動返事
    $transac_no = $_POST['order_no'];
    // TODO check other post values?

    // update webcollect status
    if(!Webcollect::transacAuto($transac_no, $_POST)) {
        header("HTTP/1.0 500 Internal Server Error\n\n");
        header("Content-Type: text/plain; charset=UTF-8\n\n");
        print "注文ステータスの更新に失敗しました。";
    }
    else {
        header("HTTP/1.0 200 OK");
        print "OK";
    }
}
else {
    // order_noがない？エラーです
    header("HTTP/1.0 400 Bad Request\n\n");
    header("Content-Type: text/plain; charset=UTF-8\n\n");
    print "注文番号がありません。";
}

// 終了
exit;