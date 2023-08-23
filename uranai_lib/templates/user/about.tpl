{extends file="main.tpl"}
{block name=seo}
<title>サイトについて 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp">
<meta name="description" content="サイト毎に違う結果が出ている12星座占い!独自に集計し星座のランキングを出しています。">

<!--OGP START-->
{include file="ogp.tpl" title="|サイトについて" des="サイト毎に違う結果が出ている12星座占い!独自に集計し星座のランキングを出しています。"}
<!--OGP END--> 
{/block}

{block name=body}
<div class="container about">
<div class="title row text-center">
	<h2 class="font-color">{$config.plateform}について</h2>
	{include "mainline.parts.tpl"}
</div>


	<div class="col-md-9 spadding-top">
		<div class="tecen base-bg contents-space">
			<p>
			Webの世界には星座占いのサイトが沢山有り、サイト毎に各星座の結果も違います。<br />
			各サイトの12星座の順位から得点を独自に設定し、集計したらどうなるんだろう？<br>
			ちょっとした好奇心からこの{$config.plateform}を作りました。<br />
			<br />
			結果はどの星座も同じ得点になるのではないか？<br />
			また、集計結果の順位と同じランキングのサイトがあるのではないか？<br />
			逆に全然違う結果を出しているサイトがあるのではないか？<br />
			集計結果は当てはまらないけどこのサイト当たっている！<br />
			などなど、色々調査をしていきたいと思っております。<br />
			<br />
			集計に加えるサイトは順次追加しております。<br />
			また、順位を出している12星座占いサイトのオーナ様！追加させて頂きますのでお気軽にご連絡下さい。<br />
			<br />
			当たるも八卦、当たらぬも八卦、予定は未定、お遊び程度に見ていって下さいね。<br />
			</p>
		</div>
	</div>
	{include file='sidebar.tpl'}
</div>
{/block}
