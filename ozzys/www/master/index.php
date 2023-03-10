<?PHP
//	OZZYS SITE 管理プログラム
//	アルファテック株式会社
//	2002.06.00
//	製作：大河原 康史
//	email：ookawara@alphatec.co.jp


//	初期設定

include './array.inc';
include './select.inc';
$title = 'OZZYS SITE 管理画面';
session_start();



//	大元画面	************************************************************************

	echo (headers());
	echo (select());

	if ($main == 1) {
		if ($goods == 1)	{ include './goods_1.inc'; goods_1(); }
		if ($goods == 2)	{ include './goods_2.inc'; goods_2(); }
		if ($goods == 3)	{ include './goods_3.inc'; goods_3(); }
		if ($goods == 4)	{ include './goods_4.inc'; goods_4(); }
		if ($goods == 5)	{ include './goods_5.inc'; goods_5(); }
		if ($goods == 6)	{ include './goods_6.inc'; goods_6(); }
		if ($goods == 7)	{ include './goods_7.inc'; goods_7(); }
		if ($goods == 8)	{ include './goods_8.inc'; goods_8(); }
		if ($goods == 9)	{ include './goods_9.inc'; goods_9(); }
		if ($goods == 10)	{ include './goods_10.inc'; goods_10(); }
		if ($goods == 11)	{ include './goods_11.inc'; goods_11(); }
		if ($goods == 12)	{ include './goods_12.inc'; goods_12(); }
	}
	elseif ($main == 2) {
		if ($kokyaku == 1)	{ include './kokyaku_1.inc'; kokyaku_1(); }
		if ($kokyaku == 2)	{ include './kokyaku_2.inc'; kokyaku_2(); }
		if ($kokyaku == 3)	{ include './kokyaku_3.inc'; kokyaku_3(); }
		if ($kokyaku == 4)	{ include './kokyaku_4.inc'; kokyaku_4(); }
	}
	elseif ($main == 3) { include './access.inc'; access(); }
	elseif ($main == 4)	{
		if ($mail == 1)	{ include './meruma_1.inc'; meruma_1(); }
		if ($mail == 2)	{ include './meruma_2.inc'; meruma_2(); }
	}
//	elseif ($main == 5)	{ include './cleen.inc'; cleen(); }
	elseif ($main == 5)	{ include './ends.inc'; ends(); }
	else {
		echo <<<ALPHA
2016/07/26<br>
○送料無料設定の動作確認の為、注文管理で送料変更のメッセージなどが表示されますが<br>
　無視し今まで通り作業を進めて下さい。<br>
　作業が完了次第改めてご連絡させて頂きます。<br>
<br>
<br>
<br>
2005/8/30<BR>
○商品情報(ポスデーター、商品詳細など)を更新した場合必ずキャッシュ削除を行ってください。<BR>
○商品にリンクを張る場合のサンプル<BR>
　商品名が同じものを表示する場合、表示したい商品番号の頭に n をつけます。　商品番号が1111の場合　例　/goods/n1111/ <BR>
　個々の商品の詳細にリンクを張る場合、表示したい商品番号の頭に g をつけます。　商品番号が2222の場合　例　/goods/g2222/ <BR>
　各メーカーの商品を表示したいときはメーカー番号の頭に m をつけます。　メーカー番号が333の場合　例　/goods/m333/ <BR>

ALPHA;
	}

	echo (footer());

exit;




//ヘッダー	************************************************************************
function headers() {
include './array.inc';
global $main,$goods,$kokyaku,$title;
?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<HTML>
<HEAD>
<META http-equiv="Content-Type" content="text/html; charset=UTF-8">
<META http-equiv="Content-Style-Type" content="text/css">
<LINK rel="stylesheet" href="style.css" type="text/css">
<TITLE><? echo ("$title"); ?></TITLE>
</HEAD>
<BODY>
<?
	echo ("$title <BR>\n");

if ($main && ($main != 0)) { echo ("$MAIN_N[$main]"); }
	if ($main == 1) {
		if ($goods && ($goods != 0)) { echo (" &gt; $GOODS_N[$goods]"); }
		}
	if ($main ==2) {
		if ($kokyaku && ($kokyaku != 0)) { echo (" &gt; $KOKYAKU_N[$kokyaku]"); }
		}
	if ($main ==4) {
		if ($mail && ($mail != 0)) { echo (" &gt; $MAIL_N[$mail]"); }
		}
	}



//フッター	************************************************************************
function footer() {
?>
</BODY>
</HTML>
<?
	}



//	ページ処理 商品管理1	************************************************************************
function next_p_g_1() {
    global $PHP_SELF,$main,$goods,$maker,$bunrui,$s_goods,$s_size,$display,$view,$views,$view_s,$view_e,$page,$max,$regi;
	global $stock;	//	add ookawara 2012/11/27

	if (defined("goods_max_num")) { $max = goods_max_num; }
    ?>
    <TABLE border="0" cellpadding="0" cellspacing="0">
        <TBODY>
            <TR>
            <?
            if ($view_s >= $views) {
                $f = $page-1;
                ?>
                <TD width="100">
                    <FORM action="<? echo("$PHP_SELF"); ?>" method="POST">
                        <INPUT type="hidden" name="main" value="<? echo("$main"); ?>">
                        <INPUT type="hidden" name="goods" value="<? echo("$goods"); ?>">
                        <INPUT type="hidden" name="maker" value="<? echo("$maker"); ?>">
                        <INPUT type="hidden" name="bunrui" value="<? echo("$bunrui"); ?>">
                        <INPUT type="hidden" name="s_goods" value="<? echo("$s_goods"); ?>">
                        <INPUT type="hidden" name="s_size" value="<? echo("$s_size"); ?>">
                        <INPUT type="hidden" name="display" value="<? echo $_POST['display']; ?>">
                        <INPUT type="hidden" name="view" value="<? echo("$view"); ?>">
                        <INPUT type="hidden" name="main_r" value="<? echo("$main"); ?>">
                        <INPUT type="hidden" name="goods_r" value="<? echo("$goods"); ?>">
                        <INPUT type="hidden" name="regi" value="<? echo("$regi"); ?>">
                        <INPUT type="hidden" name="page" value="<? echo("$f"); ?>">
                        <INPUT type="hidden" name="stock" value="<? echo $_POST['stock']; ?>">
                        <INPUT type="hidden" name="sel_free_postage" value="<? echo $_POST['sel_free_postage']; ?>">
                        <INPUT type="hidden" name="sel_rod_fee" value="<? echo $_POST['sel_rod_fee']; ?>">
                        <INPUT type="submit" value="前の<? echo("$views"); ?>">
                    </FORM>
                </TD>
                <?
            }

            if ($max > $view_e) {
                $n = $page+1;
                ?>
                <TD width="100">
                    <FORM action="<? echo("$PHP_SELF"); ?>" method="POST">
                        <INPUT type="hidden" name="main" value="<? echo("$main"); ?>">
                        <INPUT type="hidden" name="goods" value="<? echo("$goods"); ?>">
                        <INPUT type="hidden" name="maker" value="<? echo("$maker"); ?>">
                        <INPUT type="hidden" name="bunrui" value="<? echo("$bunrui"); ?>">
                        <INPUT type="hidden" name="s_goods" value="<? echo("$s_goods"); ?>">
                        <INPUT type="hidden" name="s_size" value="<? echo("$s_size"); ?>">
                        <INPUT type="hidden" name="display" value="<? echo $_POST['display']; ?>">
                        <INPUT type="hidden" name="view" value="<? echo("$view"); ?>">
                        <INPUT type="hidden" name="main_r" value="<? echo("$main"); ?>">
                        <INPUT type="hidden" name="goods_r" value="<? echo("$goods"); ?>">
                        <INPUT type="hidden" name="regi" value="<? echo("$regi"); ?>">
                        <INPUT type="hidden" name="page" value="<? echo("$n"); ?>">
                        <INPUT type="hidden" name="stock" value="<? echo $_POST['stock']; ?>">
                        <INPUT type="hidden" name="sel_free_postage" value="<? echo $_POST['sel_free_postage']; ?>">
                        <INPUT type="hidden" name="sel_rod_fee" value="<? echo $_POST['sel_rod_fee']; ?>">
                        <INPUT type="submit" value="次の<? echo("$views"); ?>">
                    </FORM>
                </TD>
                <?
            }
            ?>
            </TR>
        </TBODY>
    </TABLE>
    <?
}


//	ページ処理 商品管理2	************************************************************************
function next_p_g_2() {
    global $PHP_SELF,$main,$goods,$maker,$bunrui,$display,$year_s,$mon_s,$day_s,$year_e,$mon_e,$day_e;
    global $view,$views,$view_s,$view_e,$page,$max;
	global $stock;	//	add ookawara 2012/11/27
    ?>

    <TABLE border="0" cellpadding="0" cellspacing="0">
        <TBODY>
            <TR>
            <?
            if ($view_s >= $views) {
                $f = $page-1;
                ?>
                <TD width="100">
                    <FORM action="<? echo("$PHP_SELF"); ?>" method="POST">
                        <INPUT type="hidden" name="main" value="<? echo("$main"); ?>">
                        <INPUT type="hidden" name="main_r" value="<? echo("$main"); ?>">
                        <INPUT type="hidden" name="goods" value="<? echo("$goods"); ?>">
                        <INPUT type="hidden" name="goods_r" value="<? echo("$goods"); ?>">
                        <INPUT type="hidden" name="maker" value="<? echo("$maker"); ?>">
                        <INPUT type="hidden" name="bunrui" value="<? echo("$bunrui"); ?>">
                        <INPUT type="hidden" name="year_s" value="<? echo("$year_s"); ?>">
                        <INPUT type="hidden" name="mon_s" value="<? echo("$mon_s"); ?>">
                        <INPUT type="hidden" name="day_s" value="<? echo("$day_s"); ?>">
                        <INPUT type="hidden" name="year_e" value="<? echo("$year_e"); ?>">
                        <INPUT type="hidden" name="mon_e" value="<? echo("$mon_e"); ?>">
                        <INPUT type="hidden" name="day_e" value="<? echo("$day_e"); ?>">
                        <INPUT type="hidden" name="view" value="<? echo("$view"); ?>">
                        <INPUT type="hidden" name="page" value="<? echo("$f"); ?>">
                        <INPUT type="hidden" name="stock" value="<? echo $_POST['stock']; ?>">
                        <INPUT type="hidden" name="sel_free_postage" value="<? echo $_POST['sel_free_postage']; ?>">
                        <INPUT type="hidden" name="sel_rod_fee" value="<? echo $_POST['sel_rod_fee']; ?>">
                        <INPUT type="submit" value="前の<? echo("$views"); ?>">
                    </FORM>
                </TD>
                <?
            }

            if ($max > $view_e) {
                $n = $page+1;
                ?>
                <TD width="100">
                    <FORM action="<? echo("$PHP_SELF"); ?>" method="POST">
                        <INPUT type="hidden" name="main" value="<? echo("$main"); ?>">
                        <INPUT type="hidden" name="main_r" value="<? echo("$main"); ?>">
                        <INPUT type="hidden" name="goods" value="<? echo("$goods"); ?>">
                        <INPUT type="hidden" name="goods_r" value="<? echo("$goods"); ?>">
                        <INPUT type="hidden" name="maker" value="<? echo("$maker"); ?>">
                        <INPUT type="hidden" name="bunrui" value="<? echo("$bunrui"); ?>">
                        <INPUT type="hidden" name="year_s" value="<? echo("$year_s"); ?>">
                        <INPUT type="hidden" name="mon_s" value="<? echo("$mon_s"); ?>">
                        <INPUT type="hidden" name="day_s" value="<? echo("$day_s"); ?>">
                        <INPUT type="hidden" name="year_e" value="<? echo("$year_e"); ?>">
                        <INPUT type="hidden" name="mon_e" value="<? echo("$mon_e"); ?>">
                        <INPUT type="hidden" name="day_e" value="<? echo("$day_e"); ?>">
                        <INPUT type="hidden" name="view" value="<? echo("$view"); ?>">
                        <INPUT type="hidden" name="page" value="<? echo("$n"); ?>">
                        <INPUT type="hidden" name="stock" value="<? echo $_POST['stock']; ?>">
                        <INPUT type="hidden" name="sel_free_postage" value="<? echo $_POST['sel_free_postage']; ?>">
                        <INPUT type="hidden" name="sel_rod_fee" value="<? echo $_POST['sel_rod_fee']; ?>">
                        <INPUT type="submit" value="次の<? echo("$views"); ?>">
                    </FORM>
                </TD>
                <?
            }
            ?>
            </TR>
        </TBODY>
    </TABLE>
    <?
}


//	ページ処理 顧客管理	************************************************************************
function next_p_k() {
    global $PHP_SELF,$main,$kokyaku,$prf_,$menber,$kokyaku_r,$num_k_,$name_s_,$view,$mode,$page,$max,$view_s,$view_e,$max;
    include './array.inc';

    $views = $VIEW_NUM[$view];
    ?>

    <TABLE border="0" cellpadding="0" cellspacing="0">
        <TBODY>
            <TR>
            <?
            if ($view_s >= $views) {
                $f = $page-1;
                ?>
               <TD width="100">
                <FORM action="<? echo("$PHP_SELF"); ?>" method="POST">
                    <INPUT type="hidden" name="main" value="<? echo("$main"); ?>">
                    <INPUT type="hidden" name="main_r" value="<? echo("$main"); ?>">
                    <INPUT type="hidden" name="kokyaku" value="<? echo("$kokyaku"); ?>">
                    <INPUT type="hidden" name="kokyaku_r" value="<? echo("$kokyaku"); ?>">
                    <INPUT type="hidden" name="prf_" value="<? echo("$prf_"); ?>">
                    <INPUT type="hidden" name="num_k_" value="<? echo("$num_k_"); ?>">
                    <INPUT type="hidden" name="name_s_" value="<? echo("$name_s_"); ?>">
                    <INPUT type="hidden" name="view" value="<? echo("$view"); ?>">
                    <INPUT type="hidden" name="page" value="<? echo("$f"); ?>">
                    <INPUT type="submit" value="前の<? echo("$views"); ?>">
                </FORM>
               </TD>
                <?
            }

            if ($max > $view_e) {
                $n = $page+1;
                ?>
               <TD width="100">
                <FORM action="<? echo("$PHP_SELF"); ?>" method="POST">
                    <INPUT type="hidden" name="main" value="<? echo("$main"); ?>">
                    <INPUT type="hidden" name="main_r" value="<? echo("$main"); ?>">
                    <INPUT type="hidden" name="kokyaku" value="<? echo("$kokyaku"); ?>">
                    <INPUT type="hidden" name="kokyaku_r" value="<? echo("$kokyaku"); ?>">
                    <INPUT type="hidden" name="prf_" value="<? echo("$prf_"); ?>">
                    <INPUT type="hidden" name="num_k_" value="<? echo("$num_k_"); ?>">
                    <INPUT type="hidden" name="name_s_" value="<? echo("$name_s_"); ?>">
                    <INPUT type="hidden" name="view" value="<? echo("$view"); ?>">
                    <INPUT type="hidden" name="page" value="<? echo("$n"); ?>">
                    <INPUT type="submit" value="次の<? echo("$views"); ?>">
                </FORM>
               </TD>
                <?
            }
            ?>
            </TR>
        </TBODY>
    </TABLE>
    <?
}
?>


<?PHP	//エラー	************************************************************************
function ERROR($msr) {
	$max = count($msr);
	--$max;
	echo ("<FONT color='#ff0000'>エラー</FONT><BR>\n");
		for ($i=0; $i<=$max; ++$i) {
			echo ("<B>・ $msr[$i] </B><BR>\n");
			}
	echo ("<BR>\n");
}
?>



