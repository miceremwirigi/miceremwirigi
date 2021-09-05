            <?php
            $conn= mysqli_connect('localhost','mwirigi','qwerty123456','SmartHouse');

            // Check connection
            if (!$conn) {
              echo "Connection failed: " . mysqli_connect_error();
            }else{
            echo "Connected successfully";
            }

            //Query for getting info from database SmartHouse
            $sql = 'SELECT * FROM LiveFeed';

            //Make query and get the results
            $result = mysqli_query($conn, $sql);
            
            //Fetch the resulting rows as an array
            $rows = mysqli_fetch_all($result, MYSQLI_ASSOC);

            //free result from memory
            mysqli_free_result($result);

            //closing the connection
            mysqli_close($conn);
            
            ?>

<style >

    .content-table{
        margin-left: auto;
        margin-right: auto;
        border-collapse: collapse;
        
        font-size: 0.9en;
        min-width: 480px;
        border-radius: 8px 8px 0 0;
        overflow: hidden;
        box-shadow: 0 0 20px rgba(0, 0, 0, 0.5);
     }
    .content-table thead tr{
        background-color: #4b0082;
        color: #ffffff;
        text-align: left;
        font-weight: bold;
     }
    .content-table th,
    .content-table td{
        padding: 12px 15px;
     }
    .content-table tbody tr{
        border-bottom: 1px solid #dddddd;
     }
    .content-table tbody tr:nth-of-type(even){
        background-color: #736AFF;
     }
    .content-table tbody tr:nth-of-type(odd){
        background-color: #ee82ee;
     }
    .content-table tbody tr:last-of-type{
        border-bottom: 5px solid #6a0Dad;
        font-weight: bold;
        color: #ffffff; 
   

</style>

<html>
    <body>
        <h1 style="color:darkblue;align=center;text-align:center;">My SmartHouse Data</h1>
        <table class="content-table"cellpadding="5px" width = "70%" align="center" frame = "borders">
            <thead>
                <tr>
                    <th>HUMIDITY</th>
                    <th>TEMPERATURE</th>
                    <th>MODE</th>
                    <th>TIME</th>
                    <th>DAY</th>
                    <th>DATE</th>
               </tr>
            </thead>
            <tbody>
            <?php              
                foreach($rows as $row){?>

                    <tr>
                      <td><?php echo htmlspecialchars($row['HUMIDITY']);?></td>
                      <td><?php echo htmlspecialchars($row['TEMPERATURE']);?></td>
                      <td><?php echo htmlspecialchars($row['MODE']);?></td>
                      <td><?php echo htmlspecialchars($row['TIME']);?></td>
                      <td><?php echo htmlspecialchars($row['DAY']);?></td>
                      <td><?php echo htmlspecialchars($row['DATE']);?></td>
                    </tr>
                    
             <?php  } 
            ?>
            </tbody>
        </table>
    </body>
</html>
