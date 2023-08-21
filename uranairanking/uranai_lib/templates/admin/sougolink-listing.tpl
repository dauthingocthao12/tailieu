{extends file="main.tpl"}
{block name=body}
{literal}
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
<script>

    $(document).ready(function(){
        $(".confirm").click(function(){
            var id = $(this).data('id');
            var name = $(this).data('name');
            $("#confirm_id").val(id);  
            $("#confirm_popup").css("display", "flex");
            $("#p_confirm").append("<b>「" + name + "」</b>" + "を相互リンクとして承認いたします。よろしいですか？");
            
        });

        $(".btn_confirm").click(function(){
            var url = $('#sougo_url').val().trim();
            if(url == '') alert("相互リンクを入力してください。");
            var kana = $('#site_name_kana').val().trim();
            if(kana == '') alert("サイト名フリガナをカタカナで入力してください。");
            else $(".confirm_form").submit();
            
        });
 
         $(".delete").click(function(){
            var id = $(this).data('id');
            var name = $(this).data('name');
            $("#delete_id").val(id);
            $("#delete_popup").css("display", "flex");
             $("#p_delete").append("<b>「" + name + "」</b>" + "を削除いたします。よろしいですか？");
        });

        $(".btn_cancer").click(function(){
            $("#p_confirm").empty();
            $("#p_delete").empty();
            $("#confirm_popup").css("display", "none");
            $("#delete_popup").css("display", "none");
            $("#update_popup").css("display", "none");

        }); 

        $(".update").click(function(){
            var id = $(this).data('id');
            var name = $(this).data('name');
            var kana = $(this).data('kana');
            var link = $(this).data('url');
            var mail = $(this).data('mail');
            $("#update_site_name").val(name);  
            $("#update_site_name_kana").val(kana);  
            $("#update_their_url").val(link);  
            $("#update_id").val(id);  
            $("#update_email").val(mail); 
            $('#update_popup').css("display", "flex");

        }); 



        if ( window.history.replaceState ) {
        window.history.replaceState( null, null, window.location.href );
        }

    });

</script>
{/literal}

    {* <div id="newSougo_popup">
        <div class="newSougo_popup_content">
            <form class="button" action="index.php" method="POST">
                <p class="text-center" id="p_newSougo"></p>
                <input type="hidden" name="mode" value="sougolink">
                <input type="hidden" name="action" value="insert_newSougo">
                <input type="hidden" name="newSougo_id" id="newSougo_id">
                <label for="insert_site_name">サイト名</label>
                <input type="text" name="insert_site_name" id="insert_site_name" size="50" required><br><br>
                <label for="update_their_url">相互リンク</label>
                <input type="text" name="update_their_url" id="update_their_url" size="50" required>    
                <input type="submit" class="btn btn-info btn_update" value="更新">
            </form>
            <button class="btn btn-light btn_cancer">キャンセル</button>
        </div>
    </div> *}

    {if !empty($data)}
        <table class="table table-striped">

            <thead>
                <tr>
                    <th class="text-center">仮登録</th>
                    <th class="text-center">相互リンクID</th>
                    <th class="text-center">サイト名</th>
                    <th class="text-center">承認</th>
                    <th class="text-center">削除</th>
                </tr>
            </thead>

            <tbody>
                {foreach $data as $id => $site_info}
                    <tr>
                        <td class="text-center">{$id}</td>
                        {if $site_info["confirmed"] == 0}
                        <td class="text-center" width="150">未承認</td>
                        {else}
                        <td class="text-center" width="150">{$site_info["management_number"]}</td>
                        {/if}
                        <td class="text-center">{$site_info["site_name"]}</td> 
                        {if $site_info["confirmed"] == 1}
                            <td class="text-center"><button data-id="{$site_info["management_number"]}" data-name="{$site_info["site_name"]}" data-kana="{$sougo_confirmed_list[$site_info["management_number"]]["site_name_kana"]}" data-mail="{$sougo_confirmed_list[$site_info["management_number"]]["email"]}" data-url="{$sougo_confirmed_list[$site_info["management_number"]]["their_link"]}" class="btn btn-info update" >更新</button></td>
                        {else}
                            <td class="text-center"><button data-id="{$id}" data-name="{$site_info["site_name"]}" class="btn btn-success confirm">承認</button></td>
                        {/if}
                        <td class="text-center"><button data-id="{$id}" data-name="{$site_info["site_name"]}" class="btn btn-danger delete">削除</button></td>
                    </tr>
                {/foreach}
                
                <div id="confirm_popup">
                    <div class="confirm_popup_content">
                        <form class="button confirm_form" action="index.php" method="POST">
                            <p class="text-center" id="p_confirm"></p>
                            <p class="text-center" style="font-size:14px">サイト名フリガナ <input type="text" id="site_name_kana" name="site_name_kana" placeholder="サイト名のフリガナをカタカナで入力してください" size="50" required></p>
                            <p class="text-center" style="font-size:14px">URL <input type="text" id="sougo_url" name="sougo_url" placeholder="相互リンクを入力してください。" size="50" required></p>
                            <input type="hidden" name="mode" value="sougolink">
                            <input type="hidden" name="action" value="confirm">
                            <input type="hidden" name="confirm_id" id="confirm_id">
                            <div class="text-center">
                                <input class="btn btn-success btn_confirm" value="承認">
                                <button type="button" class="btn btn-light btn_cancer">キャンセル</button>
                            </div>
                        </form>
                    </div>
                </div>

                <div id="delete_popup">
                    <div class="delete_popup_content">
                        <form class="button" action="index.php" method="POST">
                            <p class="text-center" id="p_delete"></p>
                            <input type="hidden" name="mode" value="sougolink">
                            <input type="hidden" name="action" value="delete">
                            <input type="hidden" name="delete_id" id="delete_id">
                            <input type="submit" class="btn btn-danger btn_delete" value="削除">
                        </form>
                        <button class="btn btn-light btn_cancer">キャンセル</button>
                    </div>
                </div>

                <div id="update_popup">
                    <div class="update_popup_content">
                        <form class="button" action="index.php" method="POST">
                            <p class="text-center" id="p_update"></p>
                            <input type="hidden" name="mode" value="sougolink">
                            <input type="hidden" name="action" value="update">
                            <input type="hidden" name="update_id" id="update_id">
                            <label for="update_site_name">サイト名</label>
                            <input type="text" name="update_name" id="update_site_name" size="50" required><br><br>
                            <label for="update_site_name_kana">サイト名フリガナ</label>
                            <input type="text" name="update_site_name_kana" id="update_site_name_kana" size="50" required><br><br>
                            <label for="update_their_url">相互リンク</label>
                            <input type="text" name="update_their_url" id="update_their_url" size="50" required> <br><br>
                            <label for="update_email">メール</label>
                            <input type="text" name="update_email" id="update_email" size="50" required>     
                            <div class="text-center">
                                <input type="submit" class="btn btn-info btn_update" value="更新">
                                <button type="button" class="btn btn-light btn_cancer">キャンセル</button>
                            </div>
                        </form>
                    </div>
                </div>
            
            </tbody>

        </table>
    {else}
        <h3 class="text-center">相互リンクのサイトが存在しておりません</h3>
	{/if} 


 
{/block}

