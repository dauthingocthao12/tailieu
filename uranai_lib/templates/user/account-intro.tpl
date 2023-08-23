{extends file="main.tpl"}
{block name=seo}
<title>新規登録のご案内 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp,新規登録のご案内">
<meta name="description" content="12星座占いサイトを独自に集計しランキングを出しています。新規登録のご案内">
{/block}
{block name=body}

{if !$config.isIosApp}
<script>
<!--
$(document).ready(function(){
	$('.mailto_link').click(function(){
		var str = String.fromCharCode({$mailtoData});
		$('.mailto_link').attr('href',str);
	})
});
-->
</script>
{/if}

<div class="account-intro container">
	<div class="col-xs-12">
		<p>
			<a href="{sitelink mode="account/login"}" class="btn btn-primary"><i class="fa fa-arrow-left"></i> 戻る</a>
		</p>
		<!--new account-->

		<div class="tecen title row ">
			<h2 class="font-color">新規登録のご案内</h2>
			{include "mainline.parts.tpl"}
		</div>

		<div class="base-bg tecen contents-space">
			「12星座占いランキング」をご覧いただきましてありがとうございます。<br/>
			<br/>
			当{$config.plateform}では、ご利用頂いている皆さまのために様々なサービス提供を準備しております。<br/>
			まず、その１つとして、ユーザー登録していただいている会員様へ、１日１回ご指定の時刻に、ランキング情報をメールにてお知らせする機能がございます。<br/>
			配信メールのサンプルは、<i class="fa fa-envelope-o"></i> <a href="javascript:void(0);" class="ovlInfo_mail_ul" onclick="openRegistOverlay('block');return false;">コチラ</a> です。<br/>
			<br/>
			ご登録ご利用は、<span class="detail_default_graph_red">無料</span> です。<br/>
			<br/>
			ご登録は、空メールを送信するだけの簡単操作ですので、お気軽に是非ご利用ください。<br/>
			<br/>
		</div>
		<div class="base-bg base-bg-margin">
			<div class="contents-space">
			<h3 class="tecen">ユーザー登録手順</h3>
			
			<p class="tecen">新規ユーザー登録の手順を、以下にご案内いたします。
				<a href="{sitelink mode="kiyaku"}">利用規約</a>および<a href="{sitelink mode="policy"}">プライバシーポリシー</a>をご確認ください。</p>
			</div>
			<div class="col-xs-12 row-eq-height">
				<div class="col-md-3 col-sm-12 row-eq-height">
					<div class="panel panel-success">
						<div class="panel-heading">
							<h6 class="panel-title">１．空メールをお送りください。</h5>
						</div>
						<div class="panel-body">
							<p>当{$config.plateform}へ空メールを送っていただくだけで、メールアドレス入力を省略でき入力ミスも防げます。<br/>
							<br/>
							下記ボタンを操作すると、お使いのメールソフトが起動します。<br/>
							{if $config.isIosApp}
							<a href="{$mailtoDataClear}" class="mailto_link  btn btn-success btn-wrap" target="_blank"><i class="fa fa-envelope-o"></i> 空メール送信で登録完了!</a><br/>
							{else}
							<a href="javascript:void(0);" class="mailto_link  btn btn-success btn-wrap"><i class="fa fa-envelope-o"></i> 空メール送信で登録完了!</a><br/>
							{/if}
							<br/>
							件名も本文も、一切不要ですので、特に記入せずお送りください。<br/>
							</p>
						</div>
					</div>
					
				</div>
				
				<div class="col-md-3 col-sm-12 row-eq-height">
					<div class="panel panel-info">
						<div class="panel-heading">
							<h6 class="panel-title">２．自動返信メールを受信</h6>
						</div>
						<div class="panel-body">
							<p>自動返信メールがすぐに届きますので、メール中のURLからユーザー情報の入力画面へお進みください。<br/>
							(自動返信メールは、sender@uranairanking.jp から送信します。届かない場合は、迷惑メールフォルダもご確認ください。)</p>
						</div>
					</div>
				</div>
				<div class="col-md-4 col-sm-12 row-eq-height">
				
					<div class="panel panel-warning">
						<div class="panel-heading">
							<h6 class="panel-title">３．ユーザー情報のご入力</h6>
						</div>
						<div class="panel-body">
							<p>
							入力画面では、下記項目の設定をお願いします。<br/>
							・パスワード<br/>
							・ニックネーム<br/>
							・生年月日<br/>
							・性別<br/>
							・都道府県(任意)<br/>
							・メール配信に関する次の設定<br/>
							　配信時刻、<br/>
							　配信曜日および祝日配信の選択。
							</p>
						</div>
					</div>
					
				</div>
				<div class="col-md-2 col-sm-12 row-eq-height">
					<div class="panel panel-danger">
						<div class="panel-heading">
							<h5 class="panel-title">完了</h5>
						</div>
						<div class="panel-body">
							<p>ご入力頂いた情報は、メールアドレスを除いて、いつでも変更できます。</p>
						</div>
					</div>
				</div>
			</div>
			<p class="tecen contents-space">
			ご登録ユーザー様向けのサービスは、今後も引き続き拡充してまいります。<br/>
			少しでも皆様のお役に立つことが出来れば幸いです。<br/>
			<br/>
			皆さまのご利用をお待ちしております。
			</p>

		</div>
	</div>
</div>


<!-- オーバーラップウィンドウ start -->
<div id="ovlInfo">

<div id="ovl2Info">

<div class="ovlInfo_close">
<input type="button" value="閉じる" style="color:black;" onClick="openRegistOverlay('none');" />
</div>
<br/>

<table border="0" cellspacing="1" bgcolor="#666666">
<tr><td>
<pre>
件名：12星座占いランキング!
本文：
○○○さん、こんにちは

＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
今日の１２星座占いランキングです
2017年11月14日(火) 08:00
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

★おひつじ座
　　　総合運 :  －  12位
　　　恋愛運 :  ↑   2位
　　　仕事運 :  ↑   9位
　　　　金運 :  ↑   6位


過去3日間の「おひつじ座」ランキングの変化です。

11月13日(月)
　　　総合運: 12位
　　　恋愛運: 10位
　　　仕事運: 11位
　　　　金運:  7位
 
11月12日(日)
　　　総合運:  8位
　　　恋愛運:  7位
　　　仕事運: 10位
　　　　金運:  3位
 
11月11日(土)
　　　総合運:  2位
　　　恋愛運:  4位
　　　仕事運:  1位
　　　　金運:  5位
 


さらに詳しくはサイトへGo。
https://uranairanking.jp/

全ての運勢を簡単確認♪
下のURLから全ての星座・運勢の順位が簡単に確認できます。
※マイページへのログインが必要です。
https://uranairanking.jp/registered-person/

各占いサイトの更新時刻が異なるため、
毎日お昼ごろまでは、何回かに分けて情報を更新しています。
</pre>
</td></tr>
</table>

<div class="ovlInfo_close">
<input type="button" value="閉じる" style="color:black;" onClick="openRegistOverlay('none');" />
</div>

</div>
</div>
<!-- オーバーラップウィンドウ end -->


{/block}
