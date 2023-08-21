{extends file="main.tpl"}
{block name=body}
<h2>削除しますか？</h2>
<div class="col-sm-6 col-sm-offset-3">
	
	<div>
		<h3>タイトール</h3>
		<div class="well">{$data.news_title}</div>
		<h3>本文</h3>
		<div class="well">{$data.news_content_fetch}</div>
	</div>

	<hr>

	<div class="text-center clearfix">
		<div class="col-sm-6">
			<form name="f2" method="POST">
				<input type="hidden" name="mode" value="news">
				<input type="hidden" name="action" value="listing">
				<input type="submit" value="キャンセル" class="btn btn-warning">
			</form>
		</div>
		<div class="col-sm-6">
			<form name="f1" method="POST">
				<input type="hidden" name="mode" value="news">
				<input type="hidden" name="action" value="delete-do">
				<input type="hidden" name="id" value="{$data.news_id}">
				<input type="submit" value="OK" class="btn btn-danger">
			</form>
		</div>
	</div>
</div>
{/block}
