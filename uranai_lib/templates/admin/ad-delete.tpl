{extends file="main.tpl"}
{* block name=title}{$data.title}{/block *}
{block name=body}
{* TODO: AD-DELETE *}
<h2>削除しますか？</h2>
<div class="col-sm-6 col-sm-offset-3"><div class="well text-center clearfix">
		<div class="col-sm-6">
			<form name="f2" method="POST">
				<input type="hidden" name="mode" value="ad">
				<input type="hidden" name="action" value="listing">
				<input type="submit" value="キャンセル" class="btn btn-warning">
			</form>
		</div>
		<div class="col-sm-6">
			<form name="f1" method="POST">
				<input type="hidden" name="mode" value="ad">
				<input type="hidden" name="action" value="delete-do">
				<input type="hidden" name="id" value="{$data.ad_id}">
				<input type="submit" value="OK" class="btn btn-danger">
			</form>
		</div>
	</div></div>
	{/block}
