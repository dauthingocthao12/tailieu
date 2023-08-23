	<div class="modal fade" id="bookmark" tabindex="-1">
		<div class="modal-dialog">
			<div class="modal-content">
				<div class="modal-header">
					<button type="button" class="close" data-dismiss="modal"><span>×</span></button>
					<h4 class="modal-title fcolor-black tecen"><span class="sitename_mdl"></span>のページに移動します。よろしいですか？</h4>
				</div>
				<div class="modal-body fcolor-black">
					<span class="sitename_mdl"></span>に移動する前に、より簡単に12星座占いランキングを見るためにブックマークをしませんか？
					<div class="tecen disp_dsgn modalblock">
					
						
						<span class="manner">
							{if $config.apple_browser}
								{if $config.browser_name == "apple_chrome"}
									<span class="bm_subtitle">ブックマークのやり方</span>
									<span class="modalblock">
									メニューの右端にある <i class="fa fa-ellipsis-v" aria-hidden="true"></i> マーク→ <i class="fa fa-star-o" aria-hidden="true"></i> の順にタップ
									</span>
								{else if $config.browser_name == "safari"}
									<span class="bm_subtitle">ブックマークのやり方</span>
									<span class="modalblock">
									<img src="/user/img_re/apple_share.png" alt="共有ボタン" width="15px"> → ブックマークを追加ボタンの順にタップ
									</span>
								{/if}
							{else if $config.android_browser}
								{if $config.browser_name == "chrome"||$config.browser_name == "firefox"}
									<span class="bm_subtitle">ブックマークのやり方</span>
									<span class="modalblock">
									メニューの右端にある <i class="fa fa-ellipsis-v" aria-hidden="true"></i> マーク→ <i class="fa fa-star-o" aria-hidden="true"></i> または、「ブックマーク」の順にタップ
									</span>
								{/if}
							{else}
								{if $config.windows_browser}
									<span class="bm_subtitle">ブックマークのやり方</span>
									<span class="modalblock">
									キーボードのCtrlキーを押しながら、Dキーを押すだけで完了！
									</span>
								{else if $config.mac_browser}
									<span class="bm_subtitle">ブックマークのやり方</span>
									<span class="modalblock">キーボードの⌘キーを押しながら、Dキーを押すだけで完了！</span>
								{/if}
							{/if}
						</span>
								
					</div>
						{if $config.browser_name == "Edge" || $config.browser_name == "IE"}
							{insert ad_group id="2"}
						{else}
						<span class="tecen modalblock modal-ad">
							{insert ad_group id="2"}
						</span>
						{/if}
				</div>
				<div class="modal-footer tecen">
					<button type="button" class="btn btn-default" data-dismiss="modal">windowを閉じてブックマーク！</button>
					<span class="btn btn-primary modalurl"><a href ="" target="_blank">そのまま集計先サイトを見る</a></span>
				</div>
				<span class="tecen modalblock modal-ad">
				{if $config.browser_name == "Edge" || $config.browser_name == "IE"}
					{insert ad_group id="1"}
				{else}
					{insert ad_group id="1"}
				{/if}
				</span>
			</div>
		</div>
	</div>
	<div class="modal fade" id="url_mv_cnfrmtn" tabindex="-1">
		<div class="modal-dialog">
			<div class="modal-content">
				<div class="modal-header">
					<button type="button" class="close" data-dismiss="modal"><span>×</span></button>
					<h4 class="modal-title fcolor-black tecen"><span class="sitename_mdl"></span>のページに移動します。よろしいですか？</h4>
				</div>
				<div class="modal-body fcolor-black">
					
					<div class="tecen modalblock">
					</div>
					<span class="tecen modalblock modal-ad">
						{if $config.browser_name == "Edge" || $config.browser_name == "IE"}
							{insert ad_group id="1"}
						{else}
							{insert ad_group id="3"}
						{/if}
					</span>
				</div>
				<div class="modal-footer tecen">
					<button type="button" class="btn btn-default" data-dismiss="modal">いいえ</button>
					<span class="btn btn-primary modalurl"><a href ="" target="_blank">はい</a></span>
				</div>
				<span class="tecen modalblock modal-ad">
				{if $config.browser_name == "Edge" || $config.browser_name == "IE"}
					{insert ad_group id="1"}
				{else}
					{insert ad_group id="1"}
				</span>
				{/if}
			</div>
		</div>
	</div>

