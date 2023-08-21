{extends file="main.tpl"}
{block name=seo}
<title>サイトについて 12星座占いランキング</title>
<meta name="keywords" content="12星座占い,せいざうらない,12星座,せいざ,ランキング,uranairanking.jp">
<meta name="description" content="サイト毎に違う結果が出ている12星座占い!独自に集計し星座のランキングを出しています。">
{/block}
{block name=body}

<div class="account container">
	<div class="page-center clearfix">
		<div class="col-sm-6 col-sm-offset-3">

			{if $activateErrorNotFound}
			<div class="alert alert-danger">
					 アクティベーションキーが見つかりません
			</div>
			{/if}

			{if $activateError}
			<div class="alert alert-danger">
					アカウントのアクティベーションが出来ませんでした。
			</div>
			{/if}

			{if $activateErrorDone}
			<div class="alert alert-warning">
					アカウントのアクティベーションは既に完了しています。<br>
					<a class="alert-link" href="{sitelink mode="account/login"}">ログイン</a> して下さい。
			</div>
			{/if}

			{if $activateSuccess}
			<div class="alert alert-success">
					アカウントのアクティベーション(有効化)が完了しました。<br>
					<a class="alert-link" href="{sitelink mode="account/login"}">ログイン</a> して下さい。
			</div>
			{/if}

		</div>
	</div>
</div>

{/block}
