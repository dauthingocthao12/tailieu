{extends file="main.tpl"}
{block name=seo}
<title>ユーザー登録 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp">
<meta name="description" content="サイト毎に違う結果が出ている12星座占い!独自に集計し星座のランキングを出しています。">
{/block}
{block name=body}
{include file="google_analytics.tpl"}

<div class="account container">
	<div class="page-center clearfix">
		<div class="col-sm-6 col-sm-offset-3">

			<p><a class="btn btn-success right-space-small" href="{sitelink mode=$prev_page}"><i class="fa fa-arrow-left"></i> 元のページに戻る</a><a class="btn btn-primary" href="/"><i class="fa fa-arrow-left"></i> トップへ戻る</a></p>

			<div class="alert alert-success">
				{if !$user}
					<b> ユーザー登録が完了しました 。</b>
					<script>
					{literal}
					ga('send', 'event', 'account', 'regist');
					{/literal}
					</script>
					{if !$emailError}
					<br>確認メールを送信しました。<br><br>
					<b>ログインは<a href="/account/login/">こちら</a>から</b>
					{/if}
				{else}
					登録情報を変更しました。
				{/if}
			</div>

			{if $emailError}
			<div class="alert alert-danger">
				確認メールを送れませんでした。
			</div>
			{/if}

		</div>
	</div>
</div>

{/block}
