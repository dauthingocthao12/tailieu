{extends file="main.tpl"}
{block name=seo}
<title>運営会社 12星座占いランキング</title>
<meta name="keywords" content="12星座占いランキング,占い,ランキング,運営会社">
<meta name="description" content="12星座占いランキングのお問い合わせ">

<!--OGP START-->
{include file="ogp.tpl" title="|お問い合わせ" des="12星座占いランキングのお問い合わせ"}
<!--OGP END-->
{/block}

{block name=body}
<div class="container company">
	<div class="title row text-center">
		<h2 class="font-color">お問い合わせ</h2>
		{include "mainline.parts.tpl"}
	</div>

    <div class="col-md-9 spadding-top">
		<div class="tecen base-bg contents-space contact-thanks">
			<div>
			ありがとうございます。お問い合わせの受付が完了いたしました。<br><br>
			ご入力いただいたメールアドレス宛に、お問い合わせ内容の確認メールをお送りいたしましたのでご確認ください。<br><br>
			確認メールが届かない場合は、メールアドレスが誤っているか、迷惑メールフォルダなどに振り分けられている可能性がございますので、ご確認をお願いいたします。<br />
			自動返信メールが届かない場合は、info@uranairanking.jpまでお問い合わせください。<br><br>
			※お問い合せ内容によっては、お時間をいただく場合がございますので、あらかじめご了承ください。</p>

            <p><a class="btn btn-primary" href="/">トップに戻る</a></p>
        </div>
	</div>

    {include file='sidebar.tpl'}
</div>

{/block}

