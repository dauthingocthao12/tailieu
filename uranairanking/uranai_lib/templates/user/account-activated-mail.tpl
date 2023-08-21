{extends file="main.tpl"}
{block name=seo}
<title>メール認証完了 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp">
<meta name="description" content="サイト毎に違う結果が出ている12星座占い!独自に集計し星座のランキングを出しています。">
{/block}
{block name=body}
{if $config.is_server}
<!--コンバージョントラッキング-->
<!-- Google Code for &#12518;&#12540;&#12470;&#12540;&#30331;&#37682;&#12398;&#36948;&#25104; Conversion Page -->
<script type="text/javascript">
/* <![CDATA[ */
var google_conversion_id = 831325108;
var google_conversion_label = "lAFjCKu43nYQtIe0jAM";
var google_remarketing_only = false;
/* ]]> */
</script>
<script type="text/javascript" src="//www.googleadservices.com/pagead/conversion.js">
</script>
<noscript> <!--コンバージョントラッキング end -->
<div style="display:inline;">
<img height="1" width="1" style="border-style:none;" alt="" src="//www.googleadservices.com/pagead/conversion/831325108/?label=lAFjCKu43nYQtIe0jAM&amp;guid=ON&amp;script=0"/>
</div>
</noscript>
{/if}
<div class="account container">
	<div class="page-center clearfix">
		<div class="col-sm-6 col-sm-offset-3">
			<h2 class="font-color"><i class="fa fa-user-plus"></i> ユーザー登録</h2>
			<div class="base-bg contents-space">
				<p class="text-info">
				メールアドレスの確認が出来ました。<br>
				<br/>
				引き続き、こちらからユーザー情報の入力を行なってください。<br/>
				<p><a class="btn fontw-backb" href="{$userinfo_form}">入力フォームへ進む <i class="fa fa-arrow-right"></i></a></p>
				</p>
				<div class="alert alert-info" role="alert">
					<strong><i class="fa fa-info-circle" aria-hidden="true"></i> まだユーザー登録は完了していません。</strong>
					<p class="text-info">入力フォームへお進みになり、情報登録を完了するとアカウントが作成されます。</p>
					<small>※このページを離れてしまった場合は、期間内に再度このURLへアクセスすることで、ユーザー登録へお進みになることができます。</small>
					<hr>
					<small>※なお一週間以内に登録をご完了にならなった場合、当URLは期限切れとなりますので、ご了承ください。<br>もし期限が切れてしまった場合、お手数ですがもう一度サイト内の<br><a href="/account/intro">「新規登録のご案内」</a>ページから空メールをお送り頂くことで再び登録を行うことができます。</small>
				</div>
			</div>

		</div>
	</div>
</div>

{/block}
