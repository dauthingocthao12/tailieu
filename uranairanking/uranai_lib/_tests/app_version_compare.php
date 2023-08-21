<?php

//テスト用ファイルです。必要時以外本番環境にUPしないでください。

require_once("../libadmin/common.php");

function should_be($expected_, $result_) {
    if($expected_ != $result_) {
        return "ERROR!";
    }
    else {
        return "SUCCESS";
    }
}

// ================================
echo('TESTING VERSION COMPARISONS'.PHP_EOL);
$base_version = '1.3.0';

// android version patterns
$a = app_version_greater_than($base_version, '1.3.0');
echo("$base_version < 1.3.0 = ".($a?'YES':'NO'));
echo " : ".should_be(false, $a);
echo PHP_EOL;

$a = app_version_greater_than($base_version, '1.3');
echo("$base_version < 1.3 = ".($a?'YES':'NO')); // testing an irregular pattern
echo " : ".should_be(false, $a);
echo PHP_EOL;

$a = app_version_greater_than($base_version, '1.3.1');
echo("$base_version < 1.3.1 = ".($a?'YES':'NO'));
echo " : ".should_be(true, $a);
echo PHP_EOL;

$a = app_version_greater_than($base_version, '1.4');
echo("$base_version < 1.4 = ".($a?'YES':'NO')); // testing an irregular pattern
echo " : ".should_be(true, $a);
echo PHP_EOL;

$a = app_version_greater_than($base_version, '1.2.7');
echo("$base_version < 1.2.7 = ".($a?'YES':'NO')); // testing an irregular pattern
echo " : ".should_be(false, $a);
echo PHP_EOL;

$a = app_version_greater_than($base_version, '0.9.2');
echo("$base_version < 0.9.2 = ".($a?'YES':'NO')); // testing an irregular pattern
echo " : ".should_be(false, $a);
echo PHP_EOL;
