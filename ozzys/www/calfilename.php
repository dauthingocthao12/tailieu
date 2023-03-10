<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

 $path = 'oyasumi/';
 $file_name_end = 'oyasumi-s.jpg';

 // $first_time = '2021-11-01';
 // $file_name_this_month = date('ym', strtotime($first_time));
 $first_time = date('Y-m-01');
 $file_name_this_month = date('ym');
 $file_name_last_month = date('ym', strtotime($first_time."- 1 month"));
 $file_name_next_month = date('ym', strtotime($first_time."+ 1 month"));

 $file1 = '';
 $file2 = '';
if (file_exists($path.$file_name_this_month.$file_name_end)) {
     $file1 = $file_name_this_month;
} elseif (file_exists($path.$file_name_last_month.$file_name_end)) {
     $file1 = $file_name_last_month;
}

$file_time1 = '';
$file_time2 = '';
if ($file1) {
     $file_time1 = date ('YmdHi',filemtime($path.$file1.$file_name_end));
}
if (file_exists($path.$file_name_next_month.$file_name_end)) {
     $file2 = $file_name_next_month;
     $file_time2 = date ('YmdHi',filemtime($path.$file2.$file_name_end));
}

$html = '<A href="oyasumi/'.$file1.'oyasumi-l.jpg?var='.$file_time1.'"><IMG src="oyasumi/'.$file1.'oyasumi-s.jpg?var='.$file_time1.'" width="165" border="0"></A>';
if ($file2) {
     $html .= '<A href="oyasumi/'.$file2.'oyasumi-l.jpg?var='.$file_time2.'"><IMG src="oyasumi/'.$file2.'oyasumi-s.jpg?var='.$file_time2.'" width="165" border="0" style="margin-top:5px;"></A>';
}

echo $html;
