# xdb
C++11 wrapper over various database drivers

  Connection con("sqlite:db=db.sqlite") ;

    con.execute("INSERT INTO pages ( title, permalink ) VALUES ( ?, ? )", getValue("title"), getValue("slug")) ;


        QueryResult res = con.query("SELECT id, geom FROM tracks WHERE route=?", route_id) ;

        while ( res.next() ) {
            
            int  id = res.get<int>("id") ;
            Blob blob = res.get<Blob>("geom") ;
          
            ...
        }



