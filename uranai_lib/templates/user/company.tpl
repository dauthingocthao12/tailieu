{extends file="main.tpl"}
{block name=seo}
<title>運営会社 12星座占いランキング</title>
<meta name="keywords" content="12星座占いランキング,占い,ランキング,運営会社">
<meta name="description" content="12星座占いランキングの運営会社の紹介">

<!--OGP START-->
{include file="ogp.tpl" title="|運営会社" des="12星座占いランキングの運営会社の紹介"}
<!--OGP END-->
{/block}

{block name=body}
<div class="container company">
	<div class="title row text-center">
		<h2 class="font-color">運営会社</h2>
		{include "mainline.parts.tpl"}
	</div>


	<div class="col-md-9 spadding-top">
		<dl class="base-bg contents-space">
			<dt>会社名</dt>
			<dd><a href="http://www.azet.jp/" target="_blank" title="eラーニングシステム 有限会社アゼット">有限会社アゼット</a></dd>
			<dt>業務内容</dt>
			<dd>eラーニングシステム制作<br>
				eメーリング<br>
				WEBシステム構築<br>
				WEBコンサルティング<br>
				レンタルサーバー<br>
				ドメイン取得サービス<br>
				ショッピングサイト制作<br>
				ホームページ制作<br>
				LANネットワーク構築、設定<br>
				パソコンサポート<br>
				パソコン・ソフト販売<br>
				ロゴ・DTPデザイン<br>
				パンフレット・ポスター
			</dd>
		</dl>

		{* インターリンク アフィリエイト *}
		<div class="affiliate_wrapper">
			<div class="affiliate_area">
				{insert ad_group id="12"}
			</div>
			<div class="affiliate_area">
				{insert ad_group id="13"}
			</div>
		</div>

		</div>
	{include file='sidebar.tpl'}
</div>
{/block}
