using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Threading.Tasks;
using API.Data;
using API.Models.Search;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.TagHelpers;

namespace API.Controllers {
    [Route("api/[controller]")]
    [ApiController]
    public class SearchController : ControllerBase {
        private readonly ApplicationDbContext _database;

        public SearchController(ApplicationDbContext database) {
            _database = database;
        }

        [HttpGet("{query}")]
        public IActionResult Search(string query) {
            query = query.ToLower();
            var result = new List<SearchModel>();
            var goodsResult =
                _database.Goods.Where(x => x.Name.ToLower().Contains(query));
            foreach (var r in goodsResult) {
                result.Add(new SearchModel(r.ID, r.Name, SearchModel.ResultType.Goods));
            }

            var manResult =
                _database.Manufacturers.Where(x => x.Name.ToLower().Contains(query));
            foreach (var r in manResult) {
                result.Add(new SearchModel(r.ID, r.Name, SearchModel.ResultType.Manufacturer));
            }

            var catResult =
                _database.Categories.Where(x => x.Name.ToLower().Contains(query));
            foreach (var r in catResult) {
                result.Add(new SearchModel(r.ID, r.Name, SearchModel.ResultType.Category));
            }

            return new JsonResult(result);
        }
    }
}